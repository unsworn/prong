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
    
    <div class="header">
    <?=STR_HELP_HEADER?>
    </div>
    <br>
	<img src="images/skiss.png" width="1000">
	<br><br>
	<a href="index.php" class="button black"><?=STR_HELP_BACK?></a>
</center>
</body>
</html>