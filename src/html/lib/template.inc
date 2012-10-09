<?php

function getTemplateData()
{
    $data = file_get_contents(FL_TEMPLATE_NAME);

    return json_decode($data);
}

function getGameSprites($gameId)
{
    $result = array();
    
    $template = getTemplateData();

    foreach($template as $entry)
    {
        if ($entry->type == "graphics")
        {
            $path = FL_GAME_DIR . "/" . $gameId . "/" . $entry->name . ".png";
            array_push($result, $path);
        }
    }
    return $result;

}

function getTemplateName($name)
{
    if (isset($templateNames))
        return $templateNames[$name];
    return $name;
}

function template_strings_js()
{
    global $templateNames;
    
    echo "var templateNames = new Object();\n";
    
    if (!isset($templateNames))
        return ;

    $keys = array_keys($templateNames);

    foreach($keys as $key)
    {
        echo "templateNames.$key = \"" . $templateNames[$key] . "\";\n";
    }
}

function gameid_js($gameId)
{
    echo "var gGameId = \"$gameId\";\n";
}

function preload_js($gameId)
{
    $arr = getGameSprites($gameId);

    echo "function preload_(sketch) {\n";

    foreach($arr as $sp)
    {
        echo "\tsketch.imageCache.add(\"$sp\");\n";
    }

    echo "}\n";

}
?>