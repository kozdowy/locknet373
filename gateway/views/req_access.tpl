% for id in locks.keys():
LOCK {{hex(id)}}
<form action="/grant" method="POST">
<input name="lock" type="hidden" value="{{hex(id)}}">
<input type="submit" value="GRANT ACCESS">
</form>
<form action="/deny" method="POST">
<input name="lock" type="hidden" value="{{hex(id)}}">
<input type="submit" value="DENY ACCESS">
</form>
% end
% end
