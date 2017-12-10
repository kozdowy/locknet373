#/usr/bin/python2

from bottle import Bottle, run, get, post, request, template, redirect
from time import sleep
import sys
import os

sys.path.insert(0, '/usr/lib/python2.7/bridge')

PUB_EXP = 0x0000000000010001
PRI_EXP = 0x044de026cabdb311
MODULUS = 0xeda515ef24029417
RESIDUE = 0x859cfcfb5a1f75d5

ON_WHITELIST = 0xEE
ON_BLACKLIST = 0xBB
ON_NEITHER = 0xDD

GRANT_ACCESS = 0xA6
DENY_ACCESS = 0xAD

NUM_LOCKS = 2

LORA_REQ_ACCESS = 0xAC

def encrypt(msg):
    return pow(msg, PUB_EXP, MODULUS)

def decrypt(msg):
    return pow(msg, PRI_EXP, MODULUS)

from bridgeclient import BridgeClient as bridgeclient
bridge = bridgeclient()


locks = []
for i in range(NUM_LOCKS):
    lock = {}
    lock['whitelist'] = set()
    lock['blacklist'] = set()
    lock['req_access'] = 0

    fname = 'db/lock_{}.whitelist'.format(i)
    if os.path.isfile(fname):
        with open(fname, 'r') as f:
            for line in f:
                l = line.strip()
                if l:
                    lock['whitelist'].add(l)
    else:
        open(fname, 'a').close()

    fname = 'db/lock_{}.blacklist'.format(i)
    if os.path.isfile(fname):
        with open(fname, 'r') as f:
            for line in f:
                l = line.strip()
                if l:
                    lock['blacklist'].add(l)
    else:
        open(fname, 'a').close()

    locks.append(lock)

print(locks)

# 0 if neither
# 1 if whitelist
# 2 if blacklist
def check_lists(lock, id):
    if type(id) is long:
        return ON_NEITHER
    if id in locks[lock]['whitelist']:
        return ON_WHITELIST
    if id in locks[lock]['blacklist']:
        return ON_BLACKLIST
    return ON_NEITHER

def request_access(lock):
    locks[lock]['req_access'] = 1

def update_db():
    print(locks)
    for i in range(NUM_LOCKS):
        lock = locks[i]
        with open('db/lock_{}.whitelist'.format(i), 'w') as f:
            for id in lock['whitelist']:
                f.write('{}\n'.format(id))
        with open('db/lock_{}.blacklist'.format(i), 'w') as f:
            for id in lock['blacklist']:
                f.write('{}\n'.format(id))

def add_to_list(lock, id, l):
    n_l = 'whitelist' if l == 'blacklist' else 'blacklist'
    locks[lock][n_l].discard(id)
    locks[lock][l].add(id)
    update_db()

def remove_from_list(lock, id, l):
    locks[lock][l].discard(id)
    update_db()

app = Bottle()

@app.route('/')
def index():
    return template('index.tpl', locks=locks)

#@app.route('/req_access')
#def req_access():
    #return template("req_access.tpl", locks=locks)

    redirect('/')
def send_to_lora(lock, msg):
    locks[lock]['req_access'] = 0
    bridge.mailbox(msg)

@app.route('/grant', method='POST')
def grant():
    lock = int(request.forms.get('lock'))
    send_to_lora(lock, encrypt(GRANT_ACCESS))
    redirect('/req_access')

@app.route('/deny', method='POST')
def deny():
    lock = int(request.forms.get('lock'))
    send_to_lora(lock, encrypt(DENY_ACCESS))
    redirect('/req_access')

@app.route('/from_lora')
def from_lora():
    msg = int(request.query['msg'], 16)
    lock = request.query['lock']
    decr = decrypt(msg)
    print('recv: {}'.format(hex(msg)))
    print('decrypted: {}'.format(hex(decr)))
    if decr == LORA_REQ_ACCESS:
        request_access(lock)
    else:
	resp = check_lists(lock, decr)
        encr = encrypt(resp)
        print('resp: {}'.format(hex(resp)))
        print('encrypted: {}'.format(hex(encr)))
	bridge.mailbox(str(hex(encr))[:-1])
    return 'recv'

@app.route('/modify_list', method='POST')
def modify_list():
    id = request.forms.get('id')
    lock = int(request.forms.get('lock'))
    l = request.forms.get('list')
    addrem = request.forms.get('addrem')
    if addrem == 'add':
        add_to_list(lock, id, l)
    elif addrem == 'rem':
        remove_from_list(lock, id, l)
    redirect('/')

run(app, host='0.0.0.0', port=8080, debug=True)
