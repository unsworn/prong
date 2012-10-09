function Point(x, y)
{
    this.x = x;
    this.y = y;
}


Point.prototype.set = function(x, y)
{
    this.x = x;
    this.y = y;
}

Point.prototype.gt = function(x, y)
{
    return (this.x > x && this.y > y);
}

Point.prototype.lt = function(x, y)
{
    return (this.x < x && this.y < y);
}

//------------------------------------------------------------------------------------------------------------

function Size(w, h)
{
    this.w = w;
    this.h = h;
}

Size.prototype.empty = function()
{
    return (this.w == 0 || this.h == 0);
}

Size.prototype.set = function(w, h)
{
    this.w = w;
    this.h = h;
}

//------------------------------------------------------------------------------------------------------------

function Rect(x, y, w, h)
{
    this.origin = new Point(x,y);
    this.size   = new Size (w,h);
}

Rect.prototype.center = function()
{
    return new Point(this.origin.x + (this.size.w/2), this.origin.y + (this.size.h/2));
}

Rect.prototype.minX = function()
{
    return this.origin.x;
}

Rect.prototype.minY = function()
{
    return this.origin.y;
}

Rect.prototype.maxX = function()
{
    return this.origin.x + this.size.w;
}

Rect.prototype.maxY = function()
{
    return this.origin.y + this.size.h;
}

Rect.prototype.inside = function(x, y)
{
    return (x >= this.minX() && y >= this.minY() && x <= this.maxX() && y <= this.maxY());
}

Rect.prototype.offset = function(x, y)
{
    return new Point(x - this.origin.x, y - this.origin.y);
}

//------------------------------------------------------------------------------------------------------------

function KeyFrame()
{
    this.source = new Point(0, 0);
    this.target = new Point(0, 0);
    this.delta  = new Point(0, 0);
    this.type   = "LINEAR";
    this.time   = 1.0;
    this.rate   = 1.0;
    this.length = 1.0;
}

//------------------------------------------------------------------------------------------------------------

function Animator()
{
    this.keyframes = new Array();
}

Animator.prototype.step = function(obj, dt)
{
    if (this.keyframes.length <= 0)
        return 0.0;
        
    var k = this.keyframes[0];
   
    if (k.time >= 1.0)
    {
        this.keyframes.shift();
        k = undefined;
        if (this.keyframes.length)
            k = this.keyframes[0];
    }
   
    if (k == undefined)
        return 1.0;
        
    var time = dt;
    var t    = k.time;
    
    switch(k.type)
    {
        case "EASEIN":
        {
            time = Math.pow(t, k.rate)
            break;
        }
        case "EASEOUT":
        {
            time = Math.pow(t, 1.0/k.rate);
            break;
        }
        case "EASEINOUT":
        {
            var sign = 1;
            var r = k.rate;
            if (r%2 == 0)
                sign = -1;
            t *= 2;
            if (t < 1)
                time = 0.5 * Math.pow(t, k.rate);
            else
                time = sign*0.5 * (Math.pow(t-2, k.rate) + sign*2);
            break;
        }
        case "EXPIN":
        {
            time = (t==0) ? 0 : Math.pow(2, 10 * (t/1 - 1)) - 1 * 0.001;
            break;
        }
        case "EXPOUT":
        {
            time = (t==1) ? 1 : (-Math.pow(2, -10 * t/1) + 1);
            break;
        }
        case "EXPINOUT":
        {
            t /= 0.5;
            if (t < 1)
                time = 0.5 * Math.pow(2, 10 * (t-1));
            else
                time = 0.5 * (-Math.pow(2, -10 * (t-1)) + 2);
            break;
        }
        case "LINEAR":
        {
            time = k.time;
            break;
        }
    }
    
    obj.x = (k.source.x + k.delta.x * time);
    obj.y = (k.source.y + k.delta.y * time);
    
    k.time += (dt / k.length);
    
    return time;
}

Animator.prototype.translate  = function(ref, x, y)
{
    
}

//------------------------------------------------------------------------------------------------------------

function Node()
{
    this.position = new Point(0, 0);
    this.scale    = new Point(1, 1);
    this.angle   = 0;
    this.enabled = true;
    this.parent  = undefined;
    this.nodes   = new Array();
    this.flipper = 1;
}

Node.prototype.draw = function(p)
{
}

Node.prototype.addChild = function(n)
{
    n.parent = this;
    this.nodes.push(n);
}

Node.prototype.update = function(p, dt)
{
    this.animate(dt);
    p.pushMatrix();
    p.translate(this.position.x, this.position.y);
    p.rotate(this.angle);
    p.scale(this.flipper * this.scale.x, this.scale.y);
    this.draw(p);
    for (var i in this.nodes)
    {
        this.nodes[i].draw(p);
    }
    p.popMatrix();
}

Node.prototype.animate = function(dt)
{

}

Node.prototype.flip = function()
{
    this.flipper = -1;
}

Node.prototype.unflip = function()
{
    this.flipper = 1;
}


//------------------------------------------------------------------------------------------------------------

Sprite.prototype=new Node();
Sprite.prototype.constructor=Sprite;

function Sprite(type, name, path)
{
    Node.prototype.constructor.call(this);
    this.type     = type;
    this.name     = name;
    this.path     = path;
    this.image    = undefined;    
    this.size     = new Size(0, 0);
    this.velocity = new Point(0, 0);
    this.speed    = new Point(0, 0);
    this.mask     = true;
    this.masked   = false;
    this.scene    = undefined;
    this.properties = new Array();

}

Sprite.prototype.load = function(p)
{
    var sprite = this;
    this.image  = p.loadImage(this.path, false, function() {
        if (sprite.size.empty())
            sprite.size.set(
                sprite.image.width, 
                sprite.image.height);
    });
}

Sprite.prototype.draw = function(p)
{
    if (!this.image.loaded)
        return ;
    
    p.imageMode(p.CENTER);

    if (this.scene !== undefined && this.scene.debug)
    {
        var hx = this.size.w/2;
        var hy = this.size.h/2;

        p.stroke(0, 255, 255, 200);
        p.strokeWeight(1);
        p.noFill();
        p.rect(-hx, -hy, 2*hx, 2*hy);
    }
    p.image(
        this.image, 
        0,
        0,
        this.size.w, 
        this.size.h);

}

Sprite.prototype.addProperty = function(obj)
{
    this.properties.push(obj);
}

Sprite.prototype.getPropertyNames = function()
{
    var names = new Array();

    for (var i in this.properties)
    {
        var p = this.properties[i];
        names.push(p.name);
    }
    return names;
}

Sprite.prototype.hasProperty = function(name)
{
    for (var i in this.properties)
    {
        var p = this.properties[i];
        if (p.name == name)
            return true;
    }
    return false;
}

Sprite.prototype.getPropertyPair = function(name)
{
    for (var i in this.properties)
    {
        var p = this.properties[i];
        if (p.name == name)
            return p;
    }
    return null;
}

Sprite.prototype.getProperty = function(name)
{
    var p = this.getPropertyPair(name);
    if (p === null)
        return false;
    return p.enabled;
}

Sprite.prototype.minX = function()
{
    return (this.position.x - (this.size.w/2))
}

Sprite.prototype.maxX = function()
{
    return (this.position.x + (this.size.w/2));
}

Sprite.prototype.minY = function()
{
    return (this.position.y - (this.size.h/2));
}

Sprite.prototype.maxY = function()
{
    return (this.position.y + (this.size.h/2));
}

Sprite.prototype.intersectsLine = function(tgt, axis)
{
    
    var mx = this.maxX();
    var my = this.maxY();

    for (var i in tgt)
    {
        var p = tgt[i];
        if (axis == "Y")
        {
            if (mx >= p[0].x && (mx - this.size.w) <= p[1].x && this.position.y < p[0].y)
            {
                if (my >= p[0].y)
                {
                    return {hit:true, x:this.position.x, y:(p[0].y - this.size.h/2), line:p};
                }
            }
        }
        else if(axis == "X")
        {
            if (my > p[1].y)
            {
                if (mx >= p[0].x)
                {
                    return {hit:true, x:(p[0].x - this.size.w/2), y:this.position.y, line:p};
                }
            }

        }

    }
    return {hit:false};
}

//------------------------------------------------------------------------------------------------------------

World.prototype=new Node();
World.prototype.constructor=World;

function World(w, h)
{
    Node.prototype.constructor.call(this);
    this.size   = new Size(w, h);
    this.last   = 0;
}

World.prototype.update = function(p)
{
    var now = p.millis();
    var dt = (now - this.last) / 1000.0;
    Node.prototype.update.call(this,p,dt);
    this.last = now;
}

//------------------------------------------------------------------------------------------------------------

function Loader()
{
    this.id       = "";
    this.name     = "";
    this.imported = false;
    this.complete = false;
    this.loading  = false;
    this.sprites  = new Array();
    this.floor    = new Array();
    this.walls    = new Array();
    this.goal     = undefined;
} 

Loader.prototype.print = function() 
{
    log("Loader");
    log("Imported: " + this.imported);
    log("Complete: " + this.complete);
    log("Loading:  " + this.loading);
    log("Id:       " + this.id);
    log("Name:     " + this.name);
    log("Goal:     " + this.goal);

    for (var i in this.sprites)
    {
        var j = this.sprites[i];
        
        log("Sprite");
        log("\tName:    " + j.name);
        log("\tPath:    " + j.path);
        log("\tType:    " + j.type);
        for (var k in j.properties)
        {
            var n = j.properties[k];
            log("Property(" + n.name + ") == " + n.enabled);
        }
    }


    
}
Loader.prototype.tryload = function(p, id)
{
    if (this.loading)
        return ;

    var loader = this;

    this.loading = true;

    $.getJSON("load.php?id=" + id, function(data) {        

        log("loader try db");

        if (data["id"] == id)
        {

            loader.id   = id;
            loader.name = data.name;

            log("Loading imported");
            var b = new Sprite("background", "background", data.background.path);
            
            b.load(p);

            loader.sprites.push(b);
            
            for (var i in data.sprites)
            {
                var type = "character";

                var j = data.sprites[i];
                
//                log("Loading sprite: " + j.name);

                if (j.name == "goal")
                {
                    loader.goal = new Point(j.spnx, j.spny);
                    continue ;
                }
                else if (j.name == "main")
                    type = "player";

                var s = new Sprite(type, j.name, j.path);

                s.origin = new Point(j.spnx, j.spny);

                s.load(p);

                for (var k in j.properties)
                {
                    s.addProperty(j.properties[k]);
                }


                loader.sprites.push(s);
            }

            for (var i in data.floor)
            {
                var e = data.floor[i];
                loader.floor.push(
                    new Array( new Point(e[0].x, e[0].y), new Point(e[1].x, e[1].y) ));
            }

            for (var i in data.walls)
            {
                var e = data.walls[i];
                loader.walls.push(
                    new Array( new Point(e[0].x, e[0].y), new Point(e[1].x, e[1].y) ));
            }

            loader.imported = true;
            loader.complete = true;
        }
        else
        {
            log("Loading from template");
            loader.loadTemplate(p, id);
        }


    });

}

Loader.prototype.loadTemplate = function(p, id)
{


    if (template.boxes.length == 0)
        return ;
    
    for (var i in template.boxes)
    {
        var box = template.boxes[i];           

        var path = "data/game/" + id + "/" + box.name + ".png";
        
        var s = new Sprite(box.clz, box.name, path);

        s.load(p);

        this.sprites.push(s);
        
    }

    var loader = this;

    $.getJSON("data/game/" + id + "/properties.js", function(data) {

        for (o in data)
        {
            var attr = data[o];
            var s = loader.getSpriteByName( attr.parent );
            if (s != undefined)
                s.addProperty( attr );
        }
    });

    this.complete = true;

}

Loader.prototype.getSpriteByName = function(name)
{
    for (var i in this.sprites)
    {
        var s = this.sprites[i];
        if (s.name == name)
            return s;
    }
    return undefined;
}

Loader.prototype.getSpriteByType = function(type)
{
    for (var i in this.sprites)
    {
        var s = this.sprites[i];
        if (s.type == type)
            return s;
    }
    return undefined;
}

Loader.prototype.isLoaded = function()
{
    return this.complete;
}

