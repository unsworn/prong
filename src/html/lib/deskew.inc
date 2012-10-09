<?php

if (!defined("DESKEW_PROG"))
{
    define("DESKEW_PROG", "/var/www/prong.uidev.se/bin/deskew");

    define("DESKEW_OP_INFO",   1);
    define("DESKEW_OP_JOB",    2);
    define("DESKEW_OP_SCALE",  3);
    define("DESKEW_OP_CROP",   4);
    define("DESKEW_OP_ROTATE", 5);
}

function deskew_run_with_param($input, $op, $param)
{
    
    if (!file_exists($input))
        dir("Error, can not run without source");
    
    if (!array_key_exists("output", $param))
        die("Error, can not run without destination");

    $args = array();

    array_push($args, DESKEW_PROG);
    
    switch($op)
    {
        case DESKEW_OP_INFO:
        {
            array_push($args, "-p");
            array_push($args, "-i");
            break;
        }
        case DESKEW_OP_JOB:
        {
            if(!array_key_exists("template", $param))
                die("Error, PROCESS can not run without template");
            array_push($args, "-j");
            array_push($args, $param["template"]);
            if (array_key_exists("geometry", $param))
            {
                array_push($args, "-c");
                array_push($args, "-g");
                array_push($args, $param["geometry"]);
            }
            if (array_key_exists("angle", $param))
            {
                array_push($args, "-r");
                array_push($args, "-a");
                array_push($args, $param["angle"]);
            }
            if (array_key_exists("height", $param))
            {
                array_push($args, "-b");
                array_push($args, $param["height"]);
            }            
            break;
        }
        case DESKEW_OP_SCALE:
        {
            if (!array_key_exists("geometry", $param))
                die("Error, SCALE can not run without geometry");
            array_push($args, "-s");
            array_push($args, "-g");
            array_push($args, $param["geometry"]);
            break;
        }
        case DESKEW_OP_CROP:
        {
            if (!array_key_exists("geometry", $param))
                die("Error, CROP can not run without geometry");
            array_push($args, "-c");
            array_push($args, "-g");
            array_push($args, $param["geometry"]);            
            break;
        }
        case DESKEW_OP_ROTATE:
        {
            if (!array_key_exists("angle", $param))
                die("Error, ROTATE can not run without angle");
            array_push($args, "-r");
            array_push($args, "-a");
            array_push($args, $param["angle"]);
            break;
        }
    }

    if (array_key_exists("units", $param))
    {
        array_push($args, "-u");
        array_push($args, $param["units"]);
    }            
    
    array_push($args, "-o");
    array_push($args, $param["output"]);
    array_push($args, $input);
    
    $cmd = implode(" ", $args);

    $out = array();

    $ret = 0;

    $line = exec($cmd, $out, $ret);
    /*
    echo "<!-- generator: $cmd -->\n";
    */
    
    if ($ret != 0)
        die("Error, deskew run (" . $cmd .") failed (". $ret .") (" . $line . ") {" . implode($out) . "}");

    return $param["output"];
    
}

class ImageOp
{
    private $data   = NULL;
    private $width  = 0;
    private $height = 0;
    private $input  = "";
    private $skew   = 0;
    private $loaded = false;
    
    public function __construct($input)
    {
        $this->input = $input;
    }

    protected function load()
    {
        if ($this->loaded)
            return ;

        if (($tmp = tempnam("/tmp/", "dski")) === false)
            die("ImgOp, unable to load(), tempnam 1");
        
        $param = array("output" => $tmp, "units" => "degrees");

        $res = deskew_run_with_param($this->input, DESKEW_OP_INFO, $param);

        if (!file_exists($res))
            die("ImgOp, unable to load() run failed, no output");

        $this->data = file_get_contents($res);

        unlink($res);
        
        $json = json_decode($this->data);
        
        $this->width  = $json->width;
        $this->height = $json->height;
        $this->skew   = $json->skew;

        $this->loaded = true;
    }

    public function getWidth()
    {
        $this->load();
        return $this->width;
    }

    public function getHeight()
    {
        $this->load();
        return $this->height;
    }

    public function getSkewAngle()
    {
        $this->load();
        return $this->skew;
    }

    public function crop($x, $y, $w, $h, $output)
    {
        $param = array("output" => $output, "geometry" => $x."x".$y."+".$w."+".$h);

        $res = deskew_run_with_param($this->input, DESKEW_OP_CROP, $param);

        if (!file_exists($res))
            die("ImgOp, unable to perform crop");

        return $res;
    }

    public function scaleTo($w, $h, $output)
    {
        $param = array("output" => $output, "geometry" => "+".$w."+".$h);

        $res = deskew_run_with_param($this->input, DESKEW_OP_SCALE, $param);

        if (!file_exists($res))
            die("ImgOp, unable to perform scale");

        return $res;
    }

    public function scaleBy($sx, $sy, $output)
    {
        $this->load();
        return $this->scaleTo($this->width * $sx, $this->height * $sy, $output);
    }

    public function scaleW($w, $output)
    {        
        $this->load();

        $factor = $w / $this->width;

        $h = $this->height * $factor;

        return $this->scaleTo($w, $h, $output);
        
    }

    public function rotate($angle, $output, $degree=true)
    {
        $param = array("output" => $output, "angle" => $angle, "units" => ($degree) ? "degrees" : "radians");

        $res = deskew_run_with_param($this->input, DESKEW_OP_ROTATE, $param);

        if (!file_exists($res))
            die("ImgOp, unable to perform rotate");

        return $res;
        
    }

    public function process($angle, $height, $crop, $template, $output)
    {
        $gstr = $crop[0] . "x" . $crop[1] . "+" . $crop[2] . "+" . $crop[3];
        
        $param = array(
            "angle" => $angle,
            "height" => $height,
            "geometry" => $gstr,
            "template" => $template,
            "output" => $output);

        $res = deskew_run_with_param($this->input, DESKEW_OP_JOB, $param);

        return $res;
        
    }
    
    public function write($output)
    {
        $this->load();
        file_put_contents($output, $this->data);
    }
}
?>