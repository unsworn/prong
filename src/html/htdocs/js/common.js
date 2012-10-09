function degrees(rad)
{
    return rad * 57.2957795;
}

function radians(deg)
{
    return deg * 0.0174532925;
}

function propertyIsEnabled(val)
{
    if (typeof val == "string")
        return val == "true";
    if (typeof val == "number")
        return val == 1;
    if (typeof val == "boolean")
        return val == true;
    return false;
}

function log(msg) 
{
    if (navigator.userAgent.toLowerCase().indexOf('chrome') > -1)
        console.log(msg);
}

//------------------------------------------------------------------------------------------------------------

function Template()
{
    this.width      = 0;
    this.height     = 0;
    this.markSizeX  = 0;
    this.markSizeY  = 0;
    this.marks      = new Array();
    this.properties = new Array();
    this.boxes      = new Array();
}

Template.prototype.add = function(obj)
{

    if (obj.type == "info")
    {
        this.width = obj.width;
        this.height = obj.height;
        this.markSizeX = obj.marksx;
        this.markSizeY = obj.marksy;        
        return ;
    }

    if (obj.type == "mark")
    {
        this.marks.push(obj);
        return ;
    }

    if (obj.type == "property")
    {
        this.properties.push(obj);
        return ;
    }

    if (obj.type == "graphics")
    {
        this.boxes.push(obj);
        return ;
    }


}

//------------------------------------------------------------------------------------------------------------    

var template = new Template();
var uploadinfo = undefined;
var adjustinfo = undefined;
var soundinfo  = undefined;

//------------------------------------------------------------------------------------------------------------

function loadTemplate()
{
    $.getJSON('template.js', function(data) {
        for (o in data)
        {
            template.add(data[o]);
        }
    });
}

function loadUpload()
{
    if (gUploadId != undefined)
        $.getJSON("data/game/" + gUploadId + "/info.js", function(data) {
            uploadinfo = data;
        });
}

function loadAdjust()
{
    if (gUploadId != undefined)
        $.getJSON("data/game/" + gUploadId + "/adjusted.js", function(data) {
            adjustinfo = data;
        });
}

function importGameData(data, redir, gameId)
{
    jQuery.post("save.php", {"json": JSON.stringify(data)}, function(result) {
        alert(result);
        if (redir)
            window.location="game.php?id="+gameId;
    });
}

function loadSounds()
{
    soundinfo = new Array();

    var names = new Array("n1", "n2", "n3", "n4", "n5", "collide", "jump", "die", "loop", "walk", "fly", "scare", "lift");

    for (var n in names)
    {
        var fid = "#snd_" + names[n];
        var elem = $(fid);
        if (elem)
        {
            var player = elem.get(0);
            soundinfo.push({name: names[n], player: player});
        }
    }
}