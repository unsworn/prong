<!doctype html>

<?php
    include("config.inc");
    include("user.inc");
?>

<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
<link rel="stylesheet" type="text/css" href="css/prong.css" />
<title>Logga in</title>
</head>
<body>
    <div class="login_dialog">
        <b>Login to <?php echo APP_NAME;?></b>
        <form name="login" method="post" action="<?php echo $_SERVER['PHP_SELF'];?>">
            <table border=0>
            <tr>            
        	<td><sup>Username:</sup></td><td><input type="text" name="username"></td>
        	</tr>
        	<tr>
        	<td><sup>Password:</sup></td><td><input type="text" name="password"></td>
        	</tr>
        	<tr>
        	<td></td><td align="right"><input type="submit" value="Login"></td>
        	</tr>
            </table>
        </form>
    </div>
</body>
</html>	