% include('header.tpl')
<h2>Login</h2>
<form action="/login" method="POST">
    Username: <input name="username" type="text"> <br>
    Password: <input name="password" type="password"> <br>
    <input value="Submit" type="submit" />
</form>

<a href="/enroll">Enroll</a>
% include('footer.tpl')
