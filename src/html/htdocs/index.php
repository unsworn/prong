<?php
    include("config.inc");
    include("auth.inc");	
?>

<html>    
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
<link rel="stylesheet" type="text/css" href="css/prong.css" />
<title><?=STR_APP_NAME?></title>
</head>
<body>
<center>                      	


<?include("navigation.inc");?>
        
<div id="main_menu">         
	<table border=0>
		<tr>
			<td>
				<div class="menu_button">                            
					<a href="upload.php"><img src="images/upload.png"></a><br>
					<?=STR_MAIN_MENU_UPLOAD?>
				</div>
		    </td>
	   	    <td>
			    <div class="menu_button">
					<a href="browse.php?action=edit"><img src="images/clappy.png"></a><br>
					<?=STR_MAIN_MENU_EDIT?>
				</div>
		    </td>
        
	   	    <td>
				<div class="menu_button">
					<a href="browse.php?action=play"><img src="images/browse.png"></a><br>
					<?=STR_MAIN_MENU_BROWSE?>
				</div>
		    </td>
        </tr>
	</table>
</div>	
</center>
</body>

</html>
