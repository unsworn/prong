<?php

include("config.inc");
include("db.inc");

// include("auth.inc");

function exitWithError($err)
{
    die(json_encode(array("result" => "ERROR", "data" => $err)));
}

function convertToPointsArray($db)
{
    $r = array();
    
    while($pd = $db->fechObject())
    {
        array_push($r, convertToPoint($pd));
    }

    return $r;
    
}

function convertToPoint($arg)
{
    return array(
            array("x" => $arg->sx, "y" => $arg->sy),
            array("x" => $arg->ex, "y" => $arg->ey));    
}


header("content-type: text/javascript");

if (!isset($_REQUEST["id"]))
    exitWithError("Missing game id");

$gameId = $_REQUEST["id"];

$doImport = false;
$extId    = "";

if (isset($_REQUEST["import"]))
{
    $doImport = true;
    $extId    = $_REQUEST["import"];
}

$db = Mysql::getConnection();

$db->query("SELECT * FROM game WHERE id = '$gameId'");

if (!$db->hasResult())
    exitWithError("No game for id");


$game = $db->fetchObject();

if (!$game)
    exitWithError("Illegal argument");


$data = array();

$data["id"]   = $gameId;
$data["name"] = $game->name;
$data["path"] = FL_GAME_DIR . "/" . $gameId;

$bPath = $data["path"] . "/background.png";
$bInfo = array(0, 0);

if (file_exists($bPath))
    $bInfo = getimagesize($bPath);

$data["background"] = array(
    "path" => $bPath,
    "width" => $bInfo[0],
    "height" => $bInfo[1]);

$db->query("SELECT * FROM constraints WHERE game = '$gameId'");

$floor = array();
$walls = array();

while($o = $db->fetchObject())
{
    if ($o->type == "floor")
        array_push($floor, convertToPoint($o));
    else if($o->type == "wall")
        array_push($walls, convertToPoint($o));
}

$data["floor"] = $floor;
$data["walls"] = $walls;

$db->query("SELECT * FROM element WHERE game = '$gameId'");

$sprites = array();

while($o = $db->fetchObject())
{
    if ($doImport && $o->name == "main")
        $base = FL_GAME_DIR . "/" . $extId . "/" . $o->name;
    else
        $base = $data["path"] . "/" . $o->name;

    $ip =  $base . ".png";

    $res = array("normal" => $base . "_normal.png", "small" => $base . "_small.png", "tiny" => $base . "_tiny.png");

    $rout = array();

    if ($o->name != "goal")
    {
        foreach($res as $key => $val)
        {
            $in = getimagesize($val);
            $rout[$key] = array("path" => $val, "width" => $in[0], "height" => $in[1]);
        }
    }
    
    $in = array(0, 0);

    if (file_exists($ip))
        $in = getimagesize($ip);
    
    $s = array(
        "name"       => $o->name,
        "spnx"       => $o->sx,
        "spny"       => $o->sy,        
        "path"       => $ip,
        "res"        => $rout,
        "width"      => $in[0],
        "height"     => $in[1],
        "spawn"      => ($o->position == 1));

    array_push($sprites, $s);
    
}

$db->query("SELECT * FROM properties WHERE game = '$gameId'");

$cache = array();

while($p = $db->fetchObject())
{
    if(!array_key_exists($p->element, $cache))
        $cache[$p->element] = array();

    $a = $cache[$p->element];
        
    $v = array(
        "name"    => $p->name,
        "enabled" => ($p->enabled == 1));

    array_push($a, $v);

    $cache[$p->element] = $a;
        
}


$merge = array();

foreach ($sprites as $sprite)
{
    $s = $sprite;

    if(array_key_exists($sprite["name"], $cache))
    {
        $s["properties"] = $cache[$sprite["name"]];
        if ($sprite["name"] == "main")
            $data["background"]["properties"] = $s["properties"];
    }

    array_push($merge, $s);
}


$data["sprites"] = $merge;


echo json_encode($data);

?>