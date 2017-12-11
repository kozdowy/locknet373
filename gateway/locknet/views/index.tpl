Welcome, {{user}}! <br>

<a href="/assoc">Associate tag</a>
<h2>Blacklist</h2>
<form action="/modify_list" method="POST">
    Lock:
    % for id in locks.keys():
    <input name="lock" type="radio" value="{{id}}" {{"checked" if id == locks.keys()[0] else ""}}>{{id}}</input>
    % end
    <br>
    <input name="addrem" type="radio" value="add" checked>Add</input>
    <input name="addrem" type="radio" value="rem">Remove</input>
    <br>
    ID: <input name="id" type="text">
    <input name="list" type="hidden" value="blacklist">
    <input value="Submit" type="submit" />
</form>

<h2>Whitelist</h2>
<form action="/modify_list" method="POST">
    Lock:
    % for id in locks.keys():
    <input name="lock" type="radio" value="{{id}}" {{"checked" if id == locks.keys()[0] else ""}}>{{id}}</input>
    % end
    <br>
    <input name="addrem" type="radio" value="add" checked>Add</input>
    <input name="addrem" type="radio" value="rem">Remove</input>
    <br>
    ID: <input name="id" type="text">
    <input name="list" type="hidden" value="whitelist">
    <input value="Submit" type="submit" />
</form>

% for id in locks.keys():
    <div>
	<div>
        <h3>Lock {{id}} Whitelist members</h3>
        % for user in locks[id]['whitelist']:
        <p>{{user}}</p>
        % end
        </div>

	<div>
        <h3>Lock {{id}} Blacklist members</h3>
        % for user in locks[id]['blacklist']:
        <p>{{user}}</p>
        % end
        </div>
    </div>
% end

% include('req_access.tpl', locks=locks)
