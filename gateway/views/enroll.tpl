% include('header.tpl')
<h2>Enroll</h2>
<form action="/enroll" method="POST">
    First Name: <input name="name_first" type="text"> <br>
    Last Name: <input name="name_last" type="text"> <br>
    Username: <input name="username" type="text"> <br>
    Password: <input name="password" type="password"> <br>
    <input value="Submit" type="submit" />
</form>
% include('footer.tpl')
