
function Point(x, y)
{
    this.x = x;
    this.y = y;
    this.ox = 0;
    this.oy = 0;
    this.mlock = false;    
    this.al  = 40;    
    this.rad = 30;
    this.action = "any";
}

Point.prototype.inside = function(x, y)
{
    var minX = this.x - this.rad;
    var minY = this.y - this.rad;
    var maxX = this.x + this.rad;
    var maxY = this.y + this.rad;

    if (x >= minX && y >= minY && x <= maxX && y <= maxY)
        return true;
    return false;
}

Point.prototype.lock = function(x, y)
{
    this.mlock = true;
    this.ox = x - this.x;
    this.oy = y - this.y;
    this.al = 180;
}

Point.prototype.unlock = function()
{
    this.mlock = false;
    this.al = 40;
}

Point.prototype.draw = function(p)
{
    
    p.ellipseMode(p.CENTER);
    
    //p.noStroke();
    
    p.stroke(200, 0, 200, this.al);
    
    p.strokeWeight(3);

    p.fill(200, 0, 200, this.al/4);

    p.ellipse(this.x, this.y, this.rad, this.rad);
        
}

Point.prototype.getAction = function()
{
    return this.action;
}

Point.prototype.update = function(p, x, y)
{
    this.x = x;
    this.y = y;
    this.draw(p);
}

//------------------------------------------------------------------------------------------------------------

RPoint.prototype = new Point()
RPoint.prototype.constructor=RPoint;

function RPoint(x, y)
{
    this.x = x;
    this.y = y;
}

RPoint.prototype.draw = function(p)
{
    var iz = this.rad/2;
    
    p.ellipseMode(p.CENTER);
    
    p.noStroke();
    
    p.fill(0, 200, 0, this.al);
    
    p.ellipse(this.x, this.y, this.rad, this.rad);
    
    p.stroke(255, 255, 255, 120);
    
    p.strokeWeight(3);
    
    p.line(this.x - iz/2, this.y, this.x + iz/2, this.y);
    
    p.line(this.x, this.y - iz/2, this.x, this.y + iz/2);

}

//------------------------------------------------------------------------------------------------------------

OPoint.prototype = new Point()
OPoint.prototype.constructor=OPoint;

function OPoint(p, x, y)
{
    this.x = x;
    this.y = y;
    this.rad = 100;
    this.icw = p.loadImage("images/rcw.png");
    this.iccw = p.loadImage("images/rccw.png");
    this.imove = p.loadImage("images/move.png");
}

OPoint.prototype.inside = function(x, y)
{
    this.action = "inner";

    var r = this.rad * 0.3;

    var minX = this.x - r;
    var minY = this.y - r;
    var maxX = this.x + r;
    var maxY = this.y + r;

    if (x >= minX && y >= minY && x <= maxX && y <= maxY)
        return true;

    this.action = "outer";

    r = this.rad;

    minX = this.x - r;
    minY = this.y - r;
    maxX = this.x + r;
    maxY = this.y + r;

    if (x >= minX && y >= minY && x <= maxX && y <= maxY)
        return true;
   
    return false;
}

OPoint.prototype.draw = function(p)
{
    var sz = this.rad * 0.6;
    var iz = sz * 0.3;
    
    p.ellipseMode(p.CENTER);
    

    p.strokeWeight(1);

    p.stroke(0, 0, 0, this.al);

    p.fill(255, 0, 255, this.al);

    p.ellipse(this.x, this.y, this.rad, this.rad);

    p.stroke(0, 0, 0, this.al/2);


    p.fill(100, 0, 100, this.al * 2);
    
    p.ellipse(this.x, this.y, sz, sz);
    
    p.stroke(255, 255, 255, 120);
    
    p.strokeWeight(3);
    
    p.noFill();


    p.imageMode(p.CENTER);

    p.image(this.icw, this.x, this.y - 40, 32, 16);
    p.image(this.iccw, this.x, this.y + 40, 32, 16);
    p.image(this.imove, this.x, this.y, 50, 50);

}

//------------------------------------------------------------------------------------------------------------

BPoint.prototype = new Point();
BPoint.prototype.constructor=BPoint;

function BPoint(x, y, i)
{
    this.x = x;
    this.y = y;
    this.i = i;
}

BPoint.prototype.draw = function(p)
{    
    var iz = (this.rad/2)-2;
    
    Point.prototype.draw.call(this, p);
    
    p.stroke(255, 0, 255, 60);
    
    p.strokeWeight(1);
    
    if (this.i)        
        p.line(this.x, this.y+5, this.x, this.y+iz);
    else
        p.line(this.x, this.y - iz, this.x, this.y-5);
    
//    p.line(this.x, this.y - iz/2, this.x, this.y + iz/2);

}

//------------------------------------------------------------------------------------------------------------

SXPoint.prototype = new Point();
SXPoint.prototype.constructor=SXPoint;

function SXPoint(p, x, y)
{
    this.x = x;
    this.y = y;
    this.img = p.loadImage("images/horiz.png");
}

SXPoint.prototype.draw = function(p)
{
    var iz = 5;

    Point.prototype.draw.call(this, p);

    /*
    p.stroke(255, 255, 255, 120);
    
    p.strokeWeight(3);

    p.line(this.x-iz, this.y, this.x+iz, this.y);
    p.line(this.x, this.y-iz, this.x-iz, this.y);
    p.line(this.x, this.y+iz, this.x-iz, this.y);
    */

    p.imageMode(p.CENTER);
    p.image(this.img, this.x, this.y, 20, 20);

}

//------------------------------------------------------------------------------------------------------------

SYPoint.prototype = new Point();
SYPoint.prototype.constructor=SYPoint;

function SYPoint(p, x, y)
{
    this.x = x;
    this.y = y;
    this.img = p.loadImage("images/vert.png");
}

SYPoint.prototype.draw = function(p)
{
    var iz = 5;

    Point.prototype.draw.call(this, p);

    /*
    p.stroke(255, 255, 255, 120);
    
    p.strokeWeight(3);

    p.line(this.x, this.y-iz, this.x, this.y + iz);

    p.line(this.x-iz, this.y, this.x, this.y-iz);
    p.line(this.x, this.y-iz, this.x+iz, this.y);
    */

    p.imageMode(p.CENTER);
    p.image(this.img, this.x, this.y, 20, 20);
    
}
