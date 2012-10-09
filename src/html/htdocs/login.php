<!doctype html>

<?php
    include("config.inc");
    include("user.inc");
                 
	if (isset($_SESSION["user"]))
	{                               
		header("Location: index.php");
	}

	session_start();
	
	if (isset($_REQUEST["username"]) && isset($_REQUEST["password"]))
	{                               
		echo "Processing login request";
		
		$user = User::getUser($_REQUEST["username"], $_REQUEST["password"]);
		
		if (!is_null($user))
		{
			$_SESSION["user"] = $user;         
			header("Location: index.php");			
		}
		else
			echo "Invalid user/password";
	}
?>

<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />

<link rel="stylesheet" type="text/css" href="css/prong.css" />

<script type="text/javascript" src="http://code.jquery.com/jquery-latest.js"></script>

<script type="text/javascript">

function isOn(block)
{
    return (block.style.display == "block");
}

function toggle(block)
{
    if(isOn(block))
    {
        block.style.display = "none";
        block.style.visibility = "hidden";
    }
    else
    {
        block.style.display = "block";
        block.style.visibility = "visible";
    }
}

function validate()
{
    var form  = $("#play_form").get(0);

    var field = $("#play_form :input").get(0);

    var code =  $(field).val();

    if (code.length != 2)
    {
        alert("Ogitlig kod");
        return ;
    }
    
    form.submit();
}

$(document).ready(function() {
    



    $(".login_title").click(function() {        
        toggle($(".login_fields").get(0));        
    });

    $(".play_title").click(function() {                
        toggle($(".play_fields").get(0));
    });

});


</script>
        

<title>Logga in</title>
</head>
<body>
<center>
    <div class="logo">
        <img src="images/prong_w.png">
    </div>
    <div class="login_dialog">
        <span class="login_title">Skapa</span>
        <div class="login_fields">
        <form name="login" method="post" action="<?php echo $_SERVER['PHP_SELF'];?>">
            <table border=0>
            <tr>            
        	<td><span class="login_label">Användare:</span></td><td><input type="text" name="username"></td>
        	</tr>
        	<tr>
        	<td><span class="login_label">Hemligt:</span></td><td><input type="password" name="password"></td>
        	</tr>
        	<tr>
        	<td></td><td align="right"><input type="submit" value="Ok"></td>
        	</tr>
            </table>
        </form>
        </div>
    </div>
    <br>
    <div class="play_dialog">
        <span class="play_title">Spela</span>
        <div class="play_fields">
        <form name="play" id="play_form" method="get" action="game.php">
            <table border=0>
            <tr>            
        	<td><span class="login_label">Kod:</span></td><td><input type="text" name="id" id="id"></td>
        	</tr>
        	<tr>
            <td></td><td align="right"><input type="button" value="Ok" onClick="javascript:validate();"></td>
        	</tr>
            </table>
        </form>
        </div>
    </div>

</center>

</body>
</html>	
