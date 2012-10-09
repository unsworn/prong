<?php


function loadSounds($path)
{
    if (!is_dir($path))
        return false;

    $files = scandir($path);
    foreach($files as $file)
    {
        if (preg_match("/\.wav$/", $file))
        {
            $fid = substr($file, 0, strlen($file)-4);
            echo "<audio id='snd_$fid' src='" . $path . "/" . $file . "' preload='auto'>\n";
        }
    }
}

function getLastDirEntry($dir)
{           
	$last = NULL;
	
	if (!is_dir($dir))
		return NULL;
	$objects = scandir($dir);
	foreach($objects as $object)
	{            
		if ($object != "." && $object != ".." && filetype($dir."/".$object) == "dir")
		{
			$last = $object;
		}
	}                       
	return $last;
}

function getLastUniqueEntry($dir)
{               
	$outp = $dir;
	
	while (($e = getLastDirEntry($outp)) != NULL)
		$outp .= "/" . $e;
	
	return $outp;
}

function getParentDir($dir)
{
    $parts = explode("/", $dir);
    array_pop($parts);
    return implode("/", $parts);
}

function incrUniqueId($p)
{
	if ($p[1] != 'z')
	{
		$p = $p[0] . chr(ord($p[1]) + 1);
	}           
	else if ($p[0] != 'z')
	{
		$p = chr(ord($p[0]) + 1) . "a";
	}
	                      
	return $p;
}             

function getUniqueDir($dir)
{                   	
    $last = getLastUniqueEntry($dir);
             	          
    if ($last == $dir)
        return $dir . "/aa";

    $parts = explode("/", $last);

    $p = array_pop($parts);

    if ($p == "zz")
        return NULL;
    
	$p = incrUniqueId($p);

    array_push($parts, $p);

    return implode("/", $parts);
    
}

function getDirId($dir)
{
    $parts = explode("/", $dir);

    while (($part = array_pop($parts)) != NULL)
        if (preg_match("/[a-z]{2}/", $part))
            return $part;

    return NULL;
}

function rrmdir($dir)
{
	if (is_dir($dir))
	{
		$objects = scandir($dir);
		foreach ($objects as $object)
		{
			if ($object != "." && $object != "..")
			{
				if (filetype($dir."/".$object) == "dir")
					rrmdir($dir."/".$object); 
				else unlink($dir."/".$object);
       		}
     	}
     	reset($objects);
     	rmdir($dir);
   	}

}

?>