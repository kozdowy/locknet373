% for i in range(len(locks)):
% if locks[i]['req_access']:
LOCK {{i}}
<form action="/grant" method="POST">
<input type="hidden" value="{{i}}>
<input type="submit" value="GRANT ACCESS">
</form>
<form action="/deny" method="POST">
<input type="hidden" value="{{i}}>
<input type="submit" value="DENY ACCESS">
</form>
% end
% end
