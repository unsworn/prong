<?php
    include("config.inc");
    include("auth.inc");

    $mode = "play";

if (isset($_REQUEST["action"]))
    if ($_REQUEST["action"] == "edit")
        $mode = "edit";

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
    <?php
    if ($mode == "play")
        echo STR_BROWSE_PLAY;
    else if($mode == "edit")
        echo STR_BROWSE_EDIT;
    ?>
</div>

<h3><?=STR_BROWSE_TITLE?> <?=$curUser->group;?></h3>

<div class="game_list">

<table border="0" cellpadding="0" cellspacing="20">
<tr>        

<?php

$db = Mysql::getConnection();

$gameQuery = "";

if ($curUser->gid == 1)
    $gameQuery = "SELECT * FROM game";
else
    $gameQuery = "SELECT * FROM game WHERE grp = '$curUser->gid'";

$db->query($gameQuery);

$col = 1;

while($row = $db->fetchObject())
{
    echo "<td width='180', height='180' valign='top'>\n";
    echo "<div class='game_preview'>\n";
    echo "<table border='0' width='100%' border='0' cellpadding='0' cellspacing='5'>\n";
    echo "<tr><td align='center' valign='top'>\n";
    echo $row->name;
    echo "</td></tr><tr><td valign='middle' align='center' valign='top'>\n";
    $path = FL_GAME_DIR . "/" . $row->id . "/preview.png";

    if ($mode == "play")
        echo "<a href='game.php?id=$row->id'><img src='$path' border='0' height='128'></a>\n";
    else if($mode == "edit")
        echo "<a href='import.php?id=$row->id'><img src='$path' border='0' height='128'></a>\n";
    
    echo "</td></tr></table>\n";
    echo "</td>\n";

    if ($col%4 == 0)
    {
        echo "</tr><tr>\n";
    }
    $col ++;
    
}
    
?>
</tr>
</table>
</div>	
</center>
</body>

</html>
