<?php

include("auth.inc");
include("config.inc");
include("template.inc");

$spriteSize = array("normal" => 0.4, "small" => 0.25, "tiny" => 0.15);

function isTrue($p)
{
    if (is_string($p) && ($p == "true" || $p == "yes" || $p == "1"))
        return 1;

    if (is_numeric($p) && $p == 1)
        return 1;
    
    if (is_bool($p) && $p !== false)
        return 1;
    
    return 0;
}

function getPosition($jso, $name)
{
    foreach($jso->statics as $sprite)
    {
        if ($sprite->name == $name)
            return $sprite;
    }
    return NULL;
}

function scaleImage($inPath, $outPath, $opt)
{    
    $image = imagecreatefrompng($inPath);

    $width  = imagesx($image);
    $height = imagesy($image);

    if (array_key_exists("width", $opt))
    {        
        $nw     = $opt["width"];
        $nh     = $height * ($nw/$width);
    }
    else if (array_key_exists("height", $opt))
    {
        $nh     = $opt["height"];
        $nw     = $width * ($nh/$height);        
    }
    else if (array_key_exists("factor", $opt))
    {
        $nw     = $width * $opt["factor"];
        $nh     = $height * $opt["factor"];
    }
    else
    {
        $nw     = $width;
        $nh     = $height;
    }
    
    $tmp = imagecreatetruecolor($nw, $nh);

    $saveAlpha = false;
    
    if (array_key_exists("alpha", $opt))
    {
        imagealphablending($tmp, true);
        $t = imagecolorallocatealpha($tmp, 0, 0, 0, 127);
        imagefill($tmp, 0, 0, $t);
        $saveAlpha = true;
    }
    
    if (array_key_exists("color", $opt))
    {
        $rgb = $opt["color"];
        $c = imagecolorallocate($tmp, $rgb, $rgb, $rgb);
        imagefill($tmp, 0, 0, $c);
    }

    if (is_dir($outPath))
    {
        $info = pathinfo($inPath);
        $out = $outPath . "/" . $info["filename"];
        if (array_key_exists("suffix", $opt))
            $out .= "_" . $opt["suffix"];
        $out .= ".png";
        $outPath = $out;
    }
    
    imagecopyresampled($tmp, $image, 0, 0, 0, 0, $nw, $nh, $width, $height);

    if ($saveAlpha)
    {
        imagealphablending($tmp, false);
        imagesavealpha($tmp, true);
    }
    
    imagepng($tmp, $outPath);

    imagedestroy($tmp);
    
}

if (!isset($_POST["json"]))
    die("Error: No Data");

$js = json_decode($_POST["json"]);

if ($js == NULL || empty($js))
    die("Error: Unabled to read data");

$name = $js->name;
$game = $js->game;

$gameDir = FL_GAME_DIR . "/" . $game;


$imageMain = $gameDir . "/main.png";
$prevMain  = $gameDir . "/preview.png";

scaleImage($imageMain, $prevMain, array("width" => 200, "color" => 255));

$db = Mysql::getConnection();

$db->query("SELECT * FROM game WHERE id = '$game'");

if ($db->hasResult())
{
    $db->delete("DELETE FROM element WHERE game = '$game'");
    $db->delete("DELETE FROM constraints Where game = '$game'");
    $db->delete("DELETE FROM properties WHERE game = '$game'");
}
else
{
    //TODO: implement, sounds, mtempo from db.sql
    $db->insert(
        "INSERT INTO game VALUES('$game', $curUser->id, $curUser->gid, '$name', 0, CURRENT_TIMESTAMP, '', '')");
}


foreach($js->sprites as $sprite)
{
    
    $sx = 0;
    $sy = 0;
    $nm = $sprite->name;
    $hp = 0;
    
    if (($po = getPosition($js, $nm)) != NULL)
    {
        $hp = 1;
        $sx = $po->x;
        $sy = $po->y;
    }

    foreach($sprite->properties as $property)
    {
        
        $pn = $property->name;
        $pv = 0;

        $pv = isTrue($property->enabled);
        
        $db->insert("INSERT INTO properties VALUES('$game', '$nm', '$pn', '$pv')");
    }

    $db->insert("INSERT INTO element VALUES('$game', '$nm', $sx, $sy, '$hp')");

    $spritePath = $gameDir . "/" . $nm . ".png";
    
    if (file_exists($spritePath) && $nm != "background")
    {
        foreach($spriteSize as $sN => $sZ)
        {
            scaleImage($spritePath, $gameDir, array("factor" => $sZ, "suffix" => $sN, "alpha" => true));
        }
    }
}


$goal = $js->goal;



$db->insert("INSERT INTO element VALUES('$game', 'goal', $goal->x, $goal->y, '1')");


foreach($js->floor as $fl)
{
    $s = $fl[0];
    $e = $fl[1];

    $sx = $s->x;
    $sy = $s->y;
    $ex = $e->x;
    $ey = $e->y;
    
    $db->insert("INSERT INTO constraints VALUES('$game', 'floor', $sx, $sy, $ex, $ey)");
}


foreach($js->wall as $wl)
{
    $s = $wl[0];
    $e = $wl[1];

    $sx = $s->x;
    $sy = $s->y;
    $ex = $e->x;
    $ey = $e->y;
    
    $db->insert("INSERT INTO constraints VALUES('$game', 'wall', $sx, $sy, $ex, $ey)");
    
}


$db->close();

echo "Saved $name";

?>
