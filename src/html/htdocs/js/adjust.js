
//------------------------------------------------------------------------------------------------------------

function BBox(p, origin, iw, ih)
{
    this.origin = new Point(origin.x, origin.y);
    this.center = new Point(origin.x, origin.y);

    this.iw = iw;
    this.ih = ih;

    this.width  = this.iw;
    this.height = this.ih;

    this.angle  = radians(uploadinfo.skew);

    this.game = undefined;

    this.tl = new BPoint(0, 0, false);
    this.tr = new BPoint(0, 0, false);
    this.bl = new BPoint(0, 0, true);
    this.br = new BPoint(0, 0, true);

    this.cp = new OPoint(p, 0, 0);

    this.xp = new SXPoint(p, 0, 0);

    this.yp = new SYPoint(p, 0, 0);

    this.barg = false;

    if (adjustinfo != undefined)
    {
        this.origin.x = Number(adjustinfo.boxx);
        this.origin.y = Number(adjustinfo.boxy);
        this.width    = Number(adjustinfo.boxw);
        this.height   = Number(adjustinfo.boxh);
        this.angle    = Number(adjustinfo.angle);
        this.game     = adjustinfo.game;

    }
    else 
    {
        this.origin.x = 298;
        this.origin.y = 419;
        this.width    = 490;
        this.height   = 695;
        
    }

    this.updateFormData();
}

BBox.prototype.draw = function(p)
{

    var hX = this.width/2;
    var hY = this.height/2;

    var x = this.origin.x - this.width/2;
    var y = this.origin.y - this.height/2;


    p.noFill();

    p.strokeWeight(3);
    
    p.stroke(200, 0, 200, 80);

    p.rect(x, y, this.width, this.height);

    this.tl.update(p, x, y);
    this.tr.update(p, x+this.width, y);
    this.bl.update(p, x, y+this.height);
    this.br.update(p, x+this.width, y+this.height);

    this.cp.update(p, this.origin.x, this.origin.y);

    this.xp.update(p, x + this.width, y + this.height/2);

    this.yp.update(p, x + this.width/2, y + this.height);


    for (var i in template.boxes)
    {
        var gr = template.boxes[i];
        
        var ox = 0; //(template.markSizeX / template.width) * this.width;
        var oy = 0; //(template.markSizeY / template.height) * this.height;

        var rx = gr.relativeX * this.width;
        var ry = gr.relativeY * this.height;
        var rw = gr.relativeW * this.width;
        var rh = gr.relativeH * this.height;

        p.fill(0, 200, 255, 20);

        p.strokeWeight(1);

        p.stroke(0, 200, 255, 120);

        p.rect((x-ox) + rx, (y-oy) + ry, rw+(ox), rh+(oy));

    }

    p.copy(this.tl.x-5, this.tl.y-5, 10, 10, 0, 0, 50, 50);
    p.copy(this.tr.x-5, this.tr.y-5, 10, 10, p.width-50, 0, 50, 50);
    p.copy(this.bl.x-5, this.bl.y-5, 10, 10, 0, p.height-50, 50, 50);
    p.copy(this.br.x-5, this.br.y-5, 10, 10, p.width-50, p.height-50, 50, 50);

    p.noFill();
    p.stroke(30);
    p.rect(0, 0, 50, 50);
    p.rect(p.width-50, 0, 50, 50);
    p.rect(0, p.height-50, 50, 50);
    p.rect(p.width-50, p.height-50, 50, 50);

    if (this.barg == false)
        return ;

    p.strokeWeight(30);
    p.stroke(0, 0, 0, 100);
    p.strokeCap(p.ROUND);
    
    p.line(x + 40, this.origin.y - 100, x + (this.width - 40), this.origin.y - 100);

    var ad = degrees(this.angle);

    var f = (ad / 15.0) * ((this.width/2)-120);

    p.strokeWeight(20);
    p.strokeCap(p.SQUARE);
    p.stroke(0, 0, 0, 150);

    p.line(this.origin.x, this.origin.y - 100, this.origin.x + f, this.origin.y - 100);

    p.fill(255, 255, 255, 200);

    p.text("" + ad.toFixed(2), this.origin.x - 10, this.origin.y - 95);



}

BBox.prototype.getHandle = function(x, y)
{

    if (this.cp.inside(x, y))
        return this.cp;

    if (this.xp.inside(x, y))
        return this.xp;

    if (this.yp.inside(x, y))
        return this.yp;

    return undefined;
}

BBox.prototype.transform = function(x, y)
{
    var ax = x - this.origin.x;
    var ay = y - this.origin.y;
    var ra =   - this.angle;

    var xr = (ax * Math.cos(ra) - ay * Math.sin(ra)) + this.origin.x;
    var yr = (ax * Math.sin(ra) + ay * Math.cos(ra)) + this.origin.y;

    return [xr, yr];
}

BBox.prototype.touchBegan = function(x, y)
{

    var point = this.getHandle(x, y);
    
    if (point != undefined)
        point.lock(x, y);
}

BBox.prototype.touchEnded = function(x, y)
{
    this.cp.unlock();
    this.xp.unlock();
    this.yp.unlock();
    this.barg = false;

    this.updateFormData();
}

BBox.prototype.touchMoved = function(x, y)
{

    if (this.cp.mlock)
    {
        
        if (this.cp.getAction() == "outer")
        {
            this.barg = true;

            var dx = x - this.cp.x;
            var dr = dx / (this.width/2);
            var dg = 20 * dr;


            this.angle = radians(dg);
        }
        else
        {
            this.origin.x = x-this.cp.ox;
            this.origin.y = y-this.cp.oy;
        }

        return ;
    }
    
    if (this.xp.mlock)
    {
        var ox = this.origin.x + this.width/2;
        var dx = ox - x;
        this.width -= dx ;
        this.origin.x -= dx/2;
        return ;
    }

    if (this.yp.mlock)
    {
        var oy = this.origin.y + this.height/2;
        var dy = oy - y;
        this.height -= dy;
        this.origin.y -= dy/2;
        return ;
    }

}

BBox.prototype.getCropBox = function()
{
    var uWidth  = uploadinfo.width;
    var uHeight = uploadinfo.height;

    var pWidth  = this.iw;
    var pHeight = this.ih;

    var ptlX    = this.center.x - (pWidth/2);
    var ptlY    = this.center.y - (pHeight/2);

    var ctlX    = this.origin.x - (this.width/2);
    var ctlY    = this.origin.y - (this.height/2);


    var scaleX  = (uWidth / pWidth);
    var scaleY  = (uHeight / pHeight);
    
    var offsetX = (ctlX - ptlX);
    var offsetY = (ctlY - ptlY);

    var uX = offsetX * scaleX;
    var uY = offsetY * scaleY;

    var uW = this.width * scaleX;
    var uH = this.height * scaleY;

    //log("crop[" + uX + ", " + uY + ", " + uW + ", " + uH + "]");
    
    return [uX, uY, uW, uH];
}

BBox.prototype.updateFormData = function()
{

    var angle = this.angle;

    var crop  = this.getCropBox();

    var inputs = $('#save_form :input');

    var bx = this.origin.x;
    var by = this.origin.y;

    var bw = this.width;
    var bh = this.height;

    //log("distance [" + bx + ", " + by + ", " + bw + ", " + bh + "]");

    var bg = this.game;

    var obj = $.map(inputs, function(n, i) {

        var name = $(n).attr("name");

        if (name == "save_angle")
        {
            $(n).val("" + angle);
        }
        else if(name == "save_cropx")
        {
            $(n).val("" + crop[0].toFixed(0));
        }
        else if(name == "save_cropy")
        {
            $(n).val("" + crop[1].toFixed(0));
        }
        else if(name == "save_cropw")
        {
            $(n).val("" + crop[2].toFixed(0));
        }
        else if(name == "save_croph")
        {
            $(n).val("" + crop[3].toFixed(0));
        }
        else if(name == "save_boxx")
        {
            $(n).val("" + bx);
        }
        else if(name == "save_boxy")
        {
            $(n).val("" + by);
        }
        else if(name == "save_boxw")
        {
            $(n).val("" + bw);
        }
        else if(name == "save_boxh")
        {
            $(n).val("" + bh);
        }


    });
}

//------------------------------------------------------------------------------------------------------------

function adjuster(p)
{
    
    p.setup = function()
    {
        p.size(600, 840);

        this.bg = p.loadImage(gPreviewFile);
        
        this.fPf24 = p.createFont("pf", 24);
        this.fPf32 = p.createFont("pf", 32);
        this.fPf12 = p.createFont("pf", 10);

        this.or = new Point(p.width/2, p.height/2);

        this.tl = new Point(this.or.x - (this.bg.width/2), this.or.y - (this.bg.height/2));

        this.box = new BBox(p, this.or, this.bg.width, this.bg.height);

    }
    
    p.draw = function()
    {        
        
        var hX = p.width/2;
        var hY = p.height/2;

        p.background(100, 100, 100);

        p.pushMatrix();
        p.imageMode(p.CENTER);
        p.translate(hX, hY);
        p.rotate(this.box.angle);
        p.image(this.bg, 0, 0);
        p.popMatrix();

        p.noFill();

        p.stroke(0, 255, 0, 50);
        
        p.rect(this.tl.x-1, this.tl.y-1, this.bg.width+2, this.bg.height+2);

        p.textFont(this.fPf12);

        this.box.draw(p);

    }

    p.mousePressed = function()
    {
        this.box.touchBegan(p.mouseX, p.mouseY);
    }

    p.mouseReleased = function()
    {
        this.box.touchEnded(p.mouseX, p.mouseY);
    }

    p.mouseDragged = function()
    {
        this.box.touchMoved(p.mouseX, p.mouseY);
    }

}

//------------------------------------------------------------------------------------------------------------

$(document).ready(function()
{
    
    loadUpload();

    loadAdjust();

    loadTemplate();

    var sketch = new Processing.Sketch();    

    sketch.imageCache.add(gPreviewFile);

    sketch.imageCache.add("images/rcw.png");
    sketch.imageCache.add("images/rccw.png");
    sketch.imageCache.add("images/horiz.png");
    sketch.imageCache.add("images/vert.png");
    sketch.imageCache.add("images/move.png");

    sketch.attachFunction = adjuster;
    
    new Processing($("#adjust_canvas").get(0), sketch);

    
});
