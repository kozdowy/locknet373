#/usr/bin/python2

from bottle import Bottle, run, get, post, request, template, redirect, response
from time import sleep
from hashlib import sha256 as password_hash
import sys
import os
import random
import string
from keys import *

sys.path.insert(0, '/usr/lib/python2.7/bridge')

COOKIE_SECRET_KEY = "EECS373_LOCKNET_GO_BLUE"

GRANT_ACCESS = 0xA6
DENY_ACCESS = 0xAD

NUM_LOCKS = 2
LOCK_LIST = [0xD0, 0xD1]

ON_WHITELIST = 0
ON_BLACKLIST = 1
ON_NEITHER = 2


LORA_REQ_ACCESS = 0xAC

def encrypt(msg, lock):
    return pow(msg, PUBLIC_KEY, LOCK_MODULUS[lock])

def decrypt(msg):
    return pow(msg, GATEWAY_PRI, GATEWAY_MODULUS)

from bridgeclient import BridgeClient as bridgeclient
bridge = bridgeclient()

locks = {}
for lock_id in LOCK_LIST:
    lock = {}
    lock['whitelist'] = set()
    lock['blacklist'] = set()
    lock['req_access'] = 0

    fname = 'db/lock_{}.whitelist'.format(lock_id)
    if os.path.isfile(fname):
        with open(fname, 'r') as f:
            for line in f:
                l = line.strip()
                if l:
                    lock['whitelist'].add(l)
    else:
        open(fname, 'a').close()

    fname = 'db/lock_{}.blacklist'.format(lock_id)
    if os.path.isfile(fname):
        with open(fname, 'r') as f:
            for line in f:
                l = line.strip()
                if l:
                    lock['blacklist'].add(l)
    else:
        open(fname, 'a').close()

    locks[lock_id] = lock

print(locks)

def make_dict(keys, values):
    return dict(zip(keys, values + [None] * (len(keys) - len(values))))

user_keys = ['first', 'last', 'username', 'password', 'pass_salt', 'nfc_id']
users = []
user_file = 'db/users.list'
if os.path.isfile(user_file):
    with open(user_file, 'r') as f:
        users = [make_dict(user_keys, line.strip().split(' ')) for line in f]
else:
    open(user_file, 'a').close()

unassoc_tags = set()
fname = 'db/unassoc.tags'
if os.path.isfile(fname):
    with open(fname, 'r') as f:
        for line in f:
            l = line.strip()
            if l:
                unassoc_tags.add(line.strip())
else:
    open(fname, 'a').close()

def gen_salt():
    ALPHABET = string.ascii_letters + string.digits
    return ''.join(random.choice(ALPHABET) for i in range(15))


def check_lists(lock, uid):
    if type(uid) is long:
        return ON_NEITHER
    if uid in locks[lock]['whitelist']:
        return ON_WHITELIST
    if uid in locks[lock]['blacklist']:
        return ON_BLACKLIST
    return ON_NEITHER

def request_access(lock):
    locks[lock]['req_access'] = 1

def update_unassoc():
    print(unassoc_tags)
    with open('db/unassoc.locks', 'w') as f:
        for tag in unassoc_tags:
            f.write('{}\n'.format(tag))

def update_db():
    print(locks)
    for i in range(NUM_LOCKS):
        lock = locks[i]
        with open('db/lock_{}.whitelist'.format(i), 'w') as f:
            for uid in lock['whitelist']:
                f.write('{}\n'.format(uid))
        with open('db/lock_{}.blacklist'.format(i), 'w') as f:
            for uid in lock['blacklist']:
                f.write('{}\n'.format(uid))

def check_user_exists(username):
    return (username in [user['username'] for user in users])

def check_login(username, password):
    for user in users:
        if (user['username'] == username and
            user['password'] == password_hash(password + user['pass_salt']).hexdigest()):
            return True
    return False

def add_user(first, last, username, password, salt):
    if os.path.isfile(user_file):
        with open(user_file,'a') as f:
            f.write("{} {} {} {} {}\n".format(first, last, username, password, salt))

    users.append(make_dict(user_keys, [first, last, username, password, salt]))

def add_to_list(lock, uid, l):
    n_l = 'whitelist' if l == 'blacklist' else 'blacklist'
    locks[lock][n_l].discard(uid)
    locks[lock][l].add(uid)
    update_db()

def remove_from_list(lock, uid, l):
    locks[lock][l].discard(uid)
    update_db()

def send_to_lora(lock, msg):
    bridge.mailbox(hex(lock) + hex(msg)[:-1])

app = Bottle()

@app.route('/assoc', method=['POST', 'GET'])
def do_assoc():
    if request.method == 'POST':
        user = request.get_cookie("account", secret=COOKIE_SECRET_KEY)
        tag = request.forms.get('tag')
        for u in users:
            if u['username'] == user:
                u['nfc_id'] = tag
                unassoc_tags.discard(tag)
                update_unassoc()
                break
    if request.method == 'GET':
        return template('assoc.tpl', user=request.get_cookie("account", secret=COOKIE_SECRET_KEY), tags=unassoc_tags)


@app.route('/enroll', method=['POST', 'GET'])
def do_enroll():
    if request.method == 'POST':
        username = request.forms.get('username')
        if(check_user_exists(username)):
            return "Enrollment Failed," + username + "is taken"
        password = request.forms.get('password')
        salt = gen_salt()
        hashed = password_hash(password + salt).hexdigest()
        name_first = request.forms.get('name_first')
        name_last = request.forms.get('name_last')
        add_user(name_first, name_last, username, hashed, salt)
        response.set_cookie("account", username, secret=COOKIE_SECRET_KEY)
        redirect('/')
    if request.method == 'GET':
        return template('enroll.tpl')

@app.route('/login', method=['POST', 'GET'])
def do_login():
    if request.method == 'POST':
        username = request.forms.get('username')
        password = request.forms.get('password')
        if(check_login(username, password)):
            response.set_cookie("account", username, secret=COOKIE_SECRET_KEY)
            redirect('/')
        else:
            return "Login Failed"
    if request.method == 'GET':
        return template('login.tpl')

@app.route('/')
def index():
    user = request.get_cookie("account", secret=COOKIE_SECRET_KEY)
    if not user:
        redirect('/login')
    return template('index.tpl', user=user, locks=locks)

@app.route('/grant', method='POST')
def grant():
    lock = int(request.forms.get('lock'), 16)
    locks[lock]['req_access'] = 0
    send_to_lora(lock, encrypt(GRANT_ACCESS, lock))
    redirect('/')

@app.route('/deny', method='POST')
def deny():
    lock = int(request.forms.get('lock'), 16)
    locks[lock]['req_access'] = 0
    send_to_lora(lock, encrypt(DENY_ACCESS, lock) )
    redirect('/')

@app.route('/from_lora')
def from_lora():
    msg = int(request.query['msg'], 16)
    lock = int(request.query['lock'], 16)
    decr = decrypt(msg)
    print('recv: {}'.format(hex(msg)))
    print('decrypted: {}'.format(hex(decr)))
    resp = check_lists(lock, decr)
    if decr == ON_WHITELIST:
        send_to_lora(hex(lock), encrypt(GRANT_ACCESS, lock))
    elif decr == ON_BLACKLIST:
        send_to_lora(hex(lock), encrypt(DENY_ACCESS, lock))
    elif decr == ON_NEITHER:
        unassoc_tags.add(decr)
        request_access(lock)
    return 'recv'

@app.route('/modify_list', method='POST')
def modify_list():
    uid = request.forms.get('id')
    lock = int(request.forms.get('lock'), 16)
    l = request.forms.get('list')
    addrem = request.forms.get('addrem')
    if addrem == 'add':
        add_to_list(lock, uid, l)
    elif addrem == 'rem':
        remove_from_list(lock, uid, l)
        redirect('/')

run(app, host='0.0.0.0', port=8080, debug=True)
