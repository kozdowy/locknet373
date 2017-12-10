<h2>Blacklist</h2>
<form action="/modify_list" method="POST">
    Lock:
    % for i in range(len(locks)):
    <input name="lock" type="radio" value="{{i}}" {{"checked" if i == 0 else ""}}>{{i}}</input>
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
    % for i in range(len(locks)):
    <input name="lock" type="radio" value="{{i}}" {{"checked" if i == 0 else ""}}>{{i}}</input>
    % end
    <br>
    <input name="addrem" type="radio" value="add" checked>Add</input>
    <input name="addrem" type="radio" value="rem">Remove</input>
    <br>
    ID: <input name="id" type="text">
    <input name="list" type="hidden" value="whitelist">
    <input value="Submit" type="submit" />
</form>

% for i in range(len(locks)):
    <div>
	<div>
        <h3>Lock {{i}} Whitelist members</h3>
        % for id in locks[i]['whitelist']:
        <p>{{id}}</p>
        % end
        </div>

	<div>
        <h3>Lock {{i}} Blacklist members</h3>
        % for id in locks[i]['blacklist']:
        <p>{{id}}</p>
        % end
        </div>
    </div>
% end

% include('req_access.tpl', locks=locks)
