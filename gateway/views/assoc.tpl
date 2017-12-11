Associating tag for {{user}}
<form action="/assoc" method="POST">
Unassociated tags: <br>
% for tag in tags:
<input type="radio" name="tag" value="{{tag}}"> {{tag}}
% end
<input type="submit" value="Associate">
</form>
