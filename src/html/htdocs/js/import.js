function TextField(x, y, w, h)
{
    this.b = new Rect(x, y, w, h);

    this.text = new Array();

    for (var i in str_lbl_default)
        this.text.push(str_lbl_default[i]);

    this.focused = false;
    this.charx   = 2 + (this.text.length * 16);
    this.ccnt    = 0;
    this.cdir    = 1;
    this.clean   = true;
}

TextField.prototype.setup = function(p)
{
    this.font = p.createFont("pf", 24);
}

TextField.prototype.draw = function(p)
{
    p.strokeWeight(1);
    
    p.stroke(200);    
    p.fill(30);

    if (this.focused)
        p.rect(this.b.origin.x, this.b.origin.y, this.b.size.w, this.b.size.h);

    p.textFont(this.font);

    p.fill(200);

    p.text(this.toString(), this.b.minX()+2, this.b.maxY() - 10);

    if (!this.focused)
        return ;
    
    if (this.ccnt < 50)
        p.stroke(255);
    else 
        p.stroke(0, 0, 0, 0);

    p.line(this.b.minX() + this.charx, this.b.minY()+3, this.b.minX() + this.charx, this.b.maxY()-6);

    this.ccnt+=2;

    if (this.ccnt >= 100)
        this.ccnt = 0;

}

TextField.prototype.setText = function(strtext)
{
    this.text = new Array();
    for (var i in strtext)
        this.text.push(strtext[i]);

}

TextField.prototype.grabFocus = function()
{
    this.focused = true;
    if (this.clean)
    {
        this.text = new Array();
        this.charx = 2;
        this.clean = false;
    }
}

TextField.prototype.looseFocus = function()
{
    this.focused = false;
}

TextField.prototype.keyDown = function(p, keyCode, key)
{
    if (!this.focused)
        return ;
    
    if (keyCode == p.TAB)
    {
        this.focused = false;
        return ;
    }
    
    if (keyCode == p.BACKSPACE || keyCode == p.DELETE)
    {
        if (this.text.length)
        {
            this.text.pop();
            this.charx -= 16;
        }
        return ;
    }

    if (this.text.length > 100)
        return ;
    
    var c = key.toString();

    if (!c.match(/[a-öA-Ö0-9-\s]/))
        return ;

    this.text.push(key.toString());
    this.charx += 16;

}

TextField.prototype.toString = function()
{
    return this.text.join("");
}

PosVec.prototype=new Array();
PosVec.prototype.constructor=PosVec;

function PosVec()
{
    Array.prototype.constructor.call(this);
}

PosVec.prototype.find = function(s)
{
    for (i=0 ; i < this.length ; i++)
    {
        if (this[i].s.name == s.name)
            return this[i];
    }
    return undefined;
}

function Position(s, x, y)
{
    this.s = s;
    this.x = x;
    this.y = y;
}

function IconButton(p, name, x, y, w, h)
{

    this.name = name;
    this.img  = p.loadImage("images/" + name + ".png");
    this.b    = new Rect(x, y, w, h);
    this.hl   = false;
    this.a    = false;
    this.togg = true;

}

IconButton.prototype.draw = function(p)
{

    var c = this.b.center();

    p.noStroke();

    p.imageMode(p.CENTER);

    p.fill(255);

    p.ellipse(c.x, c.y, this.b.size.w-5, this.b.size.h-5);
    
    if (this.a)
        p.fill(255, 0, 255, 100);
    /*
    else if(this.hl)
        p.fill(0, 255, 255, 100);
    */
    p.ellipse(c.x, c.y, this.b.size.w-30, this.b.size.h-30);

    p.image(this.img, c.x, c.y, this.b.size.w, this.b.size.h);

    
    p.noFill();
    p.stroke(30);
    p.strokeWeight(5);

    if (this.hl)
        return ;

    p.ellipse(c.x, c.y, this.b.size.w, this.b.size.h);

}

IconButton.prototype.hilight = function(x, y)
{
    this.hl = this.b.inside(x, y);
}

//------------------------------------------------------------------------------------------------------------

function ToolBar(x, y, w, h)
{
    this.b = new Rect(x, y, w, h);
    this.buttons = new Array();

}

ToolBar.prototype.setup = function(p)
{
    var ox = 0;
    var bx = 60;
    var by = 60;
    var bs = 10;

    this.field = new TextField(10, (this.b.size.h - 40)/2, 300, 40);

    this.field.setup(p);

    this.buttons.push(new IconButton(p, "undo", ox, 0, bx, by));
    
    this.buttons[0].togg = false;

    ox += bx + bs;

    this.buttons.push(new IconButton(p, "target", ox, 0, bx, by));
    
    ox += bx + bs;

    this.buttons.push(new IconButton(p, "goal", ox, 0, bx, by));

    ox += bx + bs;
    
    this.buttons.push(new IconButton(p, "wall", ox, 0, bx, by));

    ox += bx + bs;

    this.buttons.push(new IconButton(p, "floor", ox, 0, bx, by));

    //this.buttons[this.buttons.length-1].a = true;

    this.bHeight = by;
    this.bWidth  = ox + bx;
    this.bSpace  = bs;

}

ToolBar.prototype.draw = function(p)
{
    
    var c = this.b.center();
    var x = this.b.maxX() - (this.bWidth + this.bSpace);
    

    p.fill(50, 50, 50);

    p.rect(this.b.origin.x, this.b.origin.y, this.b.size.w, this.b.size.h);

    p.fill(200);
    p.text(str_lbl_name_title, this.field.b.origin.x, this.field.b.origin.y-5);

    this.field.draw(p);

    p.stroke(30);

    p.strokeWeight(this.bHeight+6);

    p.line(x+30, c.y, x+(this.bWidth+30), c.y);

    p.pushMatrix();
    p.translate(x, (this.b.size.h - this.bHeight)/2);
    for (var i in this.buttons)
    {
        this.buttons[i].draw(p);
    }
    p.popMatrix();
}

ToolBar.prototype.mouseEnter = function(p)
{
}

ToolBar.prototype.mouseLeave = function(p)
{
    this.field.looseFocus();

    for (var i in this.buttons)
        this.buttons[i].hl = false;
}

ToolBar.prototype.mouseMoved = function(p, x, y)
{
    var cx = this.b.maxX() - (this.bWidth + this.bSpace);
    var cy = (this.b.size.h - this.bHeight)/2;

    for (var i in this.buttons)
        this.buttons[i].hilight(x - cx, y - cy);
}

ToolBar.prototype.mousePressed = function(p, x, y)
{
}

ToolBar.prototype.mouseReleased = function(p, x, y)
{
    var cx = this.b.maxX() - (this.bWidth + this.bSpace);
    var cy = (this.b.size.h - this.bHeight)/2;

    if (this.field.b.inside(x, y))
        this.field.grabFocus();
    else
        this.field.looseFocus();

    for (var i in this.buttons)
    {
        var button = this.buttons[i];
        if (button.b.inside(x - cx, y - cy))
        {
            if (button.togg)
            {
                this.radioOff();
                button.a = !button.a;
            }
            this.action(button);
            continue;
        }

    }
}

ToolBar.prototype.radioOff = function()
{
    for (var i=1 ; i < this.buttons.length ; i++)
        this.buttons[i].a = false;
}

ToolBar.prototype.setAction = function(cb)
{
    this.action = cb;
}


//------------------------------------------------------------------------------------------------------------

function LevelEditor(w, h)
{
    this.fontLarge;
    this.fontSmall;

    this.loaded = false;

    this.size = new Size(w, h);

    this.loader;

    this.subedit = undefined;
    this.tbar = undefined;

    this.cursor = false;
    this.lstpos = new Point(-1, -1);
    this.curpos = new Point(0, 0);

    this.floor = new Array();
    this.wall = new Array();


    this.finnish = new Point(-1, -1);
    this.statics = new PosVec();

    this.tool = "none";

}


LevelEditor.prototype.setup = function(p, s)
{
    this.subedit = s;

    this.fontLarge = p.createFont("pf", 24);
    this.fontSmall = p.createFont("pf", 12);
    
    this.loader = new Loader();

}

LevelEditor.prototype.postload = function()
{

//    this.loader.print();

    this.bg = this.loader.getSpriteByName("background");

    this.bg.size.set(this.size.w, this.size.h);

    this.loaded = true;

    if (this.loader.imported)
    {
        this.tbar.field.setText(this.loader.name);

        this.floor    = this.loader.floor;
        this.wall     = this.loader.walls;
        this.finnish  = this.loader.goal;
        
        for (var k in this.loader.sprites)
        {
            var sprite = this.loader.sprites[k];

            if (sprite["origin"] != undefined)
                this.statics.push(new Position(sprite, sprite.origin.x, sprite.origin.y));
        }
    }

    if (this.subedit != undefined)
        this.subedit.postload(this.loader);

}

LevelEditor.prototype.update = function(p)
{
    if(!this.loader.isLoaded())
    {
        p.textFont(this.fontLarge);
        p.text("Loading..", this.size.w/2 - 90, this.size.h/2 - 24);
        this.loader.tryload(p, gGameId);
        return ;
    }
    else if (!this.loaded)
    {
        this.postload();
    }

    p.noStroke();

    p.fill(80, 25, 50);

    p.rect(0, 0, this.size.w, this.size.h);
    
    p.pushMatrix();

    p.translate(this.size.w/2, this.size.h/2);

    this.bg.update(p, 0);

    p.popMatrix();

    p.fill(255, 0, 0, 100);

    p.noStroke();

    /*
    if (this.cursor)
    {
        p.ellipse(this.curpos.x, this.curpos.y, 10, 10);
    }
    */

    p.stroke(0, 255, 0, 100);

    p.strokeWeight(10);

    var np = this.floor.length ;
    
    for (var i=0 ; i < np ; i++)
    {
        var p1 = this.floor[i][0];
        var p2 = this.floor[i][1];
        p.line(p1.x, p1.y, p2.x, p2.y);
    }

    p.stroke(255, 0, 0, 100);

    np = this.wall.length ;
    
    for (var i=0 ; i < np ; i++)
    {
        var p1 = this.wall[i][0];
        var p2 = this.wall[i][1];
        p.line(p1.x, p1.y, p2.x, p2.y);
    }
    
    if (this.cursor && this.lstpos.gt(0, 0))
    {
        switch(this.tool)
        {
        case "floor":
            p.stroke(0, 255, 0, 100);
            p.line(this.lstpos.x, this.lstpos.y, this.curpos.x, this.lstpos.y);
            break;
        case "wall":
            p.line(this.lstpos.x, this.lstpos.y, this.lstpos.x, this.curpos.y);
            break;
        }
    }
    

    if (this.finnish.gt(0, 0))
    {
        p.noFill();
        p.textFont(this.fontSmall);
        p.stroke(200, 200, 200, 120);
        p.strokeWeight(2);
        p.ellipse(this.finnish.x, this.finnish.y, 50, 50);
        p.stroke(20, 20, 120);
        p.ellipse(this.finnish.x, this.finnish.y, 40, 40);
        p.fill(200);
        p.text(str_lbl_goal, this.finnish.x-10, this.finnish.y+5);
    }
    


    for (i=0 ; i < this.statics.length ; i++)
    {

        var pos = this.statics[i];

        p.pushMatrix();
        p.translate(pos.x, pos.y);
        p.scale(0.5, 0.5);
        pos.s.draw(p);
        p.popMatrix();
    }
}

LevelEditor.prototype.mouseEnter = function(p)
{
    this.cursor = true;
}

LevelEditor.prototype.mouseLeave = function(p)
{
    this.cursor = false;
}

LevelEditor.prototype.mouseMoved = function(p, x, y)
{
    this.curpos.set(x, y);
}

LevelEditor.prototype.mousePressed = function(p, x, y)
{    
    if (p.mouseButton == p.LEFT)
    {

        if (this.lstpos.lt(0, 0) && (this.tool == "floor" || this.tool == "wall"))
        {
            this.lstpos = new Point(x, y);
        }
        else            
        {
            
            switch(this.tool)
            {
            case "floor":
                this.floor.push(new Array(new Point(this.lstpos.x, this.lstpos.y), new Point(x, this.lstpos.y)));
                break;
            case "wall":
                this.wall.push(new Array(new Point(this.lstpos.x, this.lstpos.y), new Point(this.lstpos.x, y)));
                break;
            case "goal":
                this.finnish = new Point(x, y);
                break;
            case "place": {                
                var s   = this.subedit.sprites[this.subedit.editidx];
                var ps  = this.statics.find(s);

                if (ps == undefined)
                {
                    this.statics.push(new Position(s, x, y));
                }
                else
                {
                    ps.x = x;
                    ps.y = y;
                }
                break;
            }
            }
            this.lstpos.set(-1, -1);
        }
    }

}

LevelEditor.prototype.mouseReleased = function(p, x, y)
{
}

LevelEditor.prototype.mouseDragged = function(p, x, y)
{
}

LevelEditor.prototype.handleButton = function(button)
{
    switch(button.name)
    {
    case "undo":
        this.undoLast();
        break;
    case "target":
        this.tool = "place";
        break;
    case "goal":
        this.tool = "goal";
        break;
    case "wall":
        this.tool = "wall";
        break;
    case "floor":
        this.tool = "floor";
        break;
    }
}

LevelEditor.prototype.undoLast = function()
{
    switch(this.tool)
    {
    case "floor":
        if (this.loader.imported)
            this.floor = new Array();
        else
            this.floor.pop();
        break;
    case "wall":
        if (this.loader.imported)
            this.wall = new Array();
        else
            this.wall.pop();
        break;
    case "place":
        this.statics.pop();
        break;
    }
}

LevelEditor.prototype.saveExit = function()
{
    var sd = new Object();

    sd.name  = this.tbar.field.toString();
    sd.game  = gGameId;
    sd.floor = this.floor;
    sd.wall  = this.wall;
    sd.goal  = this.finnish;
    sd.statics = new Array();

    for (i=0 ; i < this.statics.length ; i++)
    {
        var st = this.statics[i];
        var so = new Object();
        so.name = st.s.name;
        so.x    = st.x;
        so.y    = st.y;
        sd.statics.push(so);
    }

    sd.sprites = new Array();

    for (var i in this.subedit.sprites)
    {
        var s  = this.subedit.sprites[i];
        var op = new Object();
        op.name = s.name;
        op.properties = new Array();
        for (var j in s.properties)
        {
            var sp = s.properties[j];
            var tmp = new Object();
            tmp.name = sp.name;
            tmp.enabled = sp.enabled;
            op.properties.push(tmp);
        }

        sd.sprites.push(op);        
    }

    importGameData(sd, true, gGameId);
    
}

//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------

function PropertyEditor(w, h)
{
    this.fontSmall = undefined;
    this.loader = undefined;
    this.size   = new Size(w, h);
    this.editidx = 0;
    this.sprites = new Array();

    this.rprev = new Rect(10, 35, 40, 150);
    this.rnext = new Rect(200, 35, 40, 150);


    this.nimg;
    this.pimg;

}

PropertyEditor.prototype.setup = function(p)
{
    this.fontSmall = p.createFont("Helvetica", 12);

    this.rprev.hilight = false;
    this.rnext.hilight = false;

    this.nimg = p.loadImage("images/next.png");
    this.pimg = p.loadImage("images/prev.png");
    
}

PropertyEditor.prototype.postload = function(l)
{
    for (var i in l.sprites)
    {
        var s = l.sprites[i];

        if (s.name != "background")
        {
            switch(s.type)
            {
            case "player":
                s.size.set(150, 150);
                break;
            case "character":
                s.size.set(120, 150);
                break;
            }
            s.edit = false;
            s.hilight = false;

            this.sprites.push(s);
            if (s.name == "main")
                this.editidx = this.sprites.length-1;
        }                
    }


}

PropertyEditor.prototype.mkprops = function()
{
    var s = this.sprites[this.editidx];

    if (this.edit)
        return ;

    var gx=0;
    var gy=0;

    var names = s.getPropertyNames();

    names.sort();

    for (var j in names)
    {
        var prop = s.getPropertyPair(names[j]);
        
        prop.bounds = new Rect(gx, gy, 80, 20);
        
        gy+=30;
        
        if (gy > 120)
        {
            gy = 0;
            gx += 100;
        }
    }

    s.edit = true;
}

PropertyEditor.prototype.update = function(p)
{
    var c;

    p.textFont(this.fontSmall);

    p.noStroke();

    if (this.sprites.length == 0)
    {
        return ;
    }

    p.fill(40, 40, 40);
    
    p.rect(0, 0, this.size.w, this.size.h);

    if (this.rprev.hilight)
        p.fill(120, 120, 120, 120);
    else
        p.fill(80, 80, 80, 120);

    p.imageMode(p.CENTER);

    //p.rect(this.rprev.origin.x, this.rprev.origin.y, this.rprev.size.w, this.rprev.size.h);
    
    c = this.rprev.center();

    p.ellipse(c.x+(this.rprev.size.w/2), c.y, this.rprev.size.w*2, this.rprev.size.h);

    p.image(this.pimg, c.x, c.y, 30);

    if (this.rnext.hilight)
        p.fill(120, 120, 120, 120);
    else
        p.fill(80, 80, 80, 120);
    
    // p.rect(this.rnext.origin.x, this.rnext.origin.y, this.rnext.size.w, this.rnext.size.h);
        
    c = this.rnext.center();

    p.ellipse(c.x-(this.rnext.size.w/2), c.y, this.rnext.size.w*2, this.rnext.size.h);

    p.image(this.nimg, c.x, c.y, 30);
    
    p.stroke(80);

    p.strokeWeight(1);

    p.line(0, 23, this.size.w, 23);

    p.fill(255);

    var s = this.sprites[this.editidx];

    p.text(str_lbl_sprite_title + ": " + templateNames[s.name], 50, 18);

    p.noStroke();
    p.fill(255, 255, 255);

    p.rect(50, 35, 150, 150);

    p.pushMatrix()
    p.translate(125, 110);

    s.update(p, 0);
    p.popMatrix();

    var np = s.properties.length; 

    if (np == 0)
        return ;

    this.mkprops();

    p.text(str_lbl_properties + ":", 250, 18);

    p.pushMatrix();

    p.translate(260, 40);


    for (var i in s.properties)
    {
        var prop = s.properties[i];
        
        var c = new Point(prop.bounds.origin.x + 10, prop.bounds.origin.y + (prop.bounds.size.h/2));

        p.noFill();

        if (prop.hilight)
            p.stroke(255);
        else
            p.stroke(200);

        p.ellipse(c.x, c.y, 20, 20);

        if (propertyIsEnabled(prop.enabled))
        {
            p.noStroke();
            p.fill(180);
            p.ellipse(c.x, c.y, 12, 12);
        }

        p.fill(180, 200, 255);
        p.text(templateNames[prop.name], c.x+17, c.y+4);

    }
    
    p.popMatrix();

}

PropertyEditor.prototype.mouseEnter = function(p)
{
}

PropertyEditor.prototype.mouseLeave = function(p)
{
}

PropertyEditor.prototype.mouseMoved = function(p, x, y)
{
    if (this.rprev.inside(x, y))
    {
        this.rprev.hilight = true;
    }
    else if (this.rnext.inside(x, y))
    {
        this.rnext.hilight = true;
    }
    else
    {
        this.rprev.hilight = this.rnext.hilight = false;
    }

    var s = this.sprites[this.editidx];


    for (var i in s.properties)
    {
        var prop = s.properties[i];
        
        if (prop.bounds.inside(x-260, y-40))
        {
            prop.hilight = true;
            continue;
        }

        prop.hilight = false;

    }

}

PropertyEditor.prototype.mousePressed = function(p, x, y)
{
    
}

PropertyEditor.prototype.mouseReleased = function(p, x, y)
{
    if (this.rprev.inside(x, y))
    {
        if (this.editidx > 0)
            this.editidx--;
        else 
            this.editidx = (this.sprites.length-1);

        this.mkprops();

        return ;
    }
    else if (this.rnext.inside(x, y))
    {
        if (this.editidx < (this.sprites.length-1))
            this.editidx++;
        else
            this.editidx = 0;
        
        this.mkprops();

        return ;
    }
    
    var s = this.sprites[this.editidx];


    for (var i in s.properties)
    {
        var prop = s.properties[i];

        if (prop.bounds.inside(x-260, y-40))
        {
            prop.enabled = !prop.enabled;

        }

    }
    
}

PropertyEditor.prototype.mouseDragged = function(p, x, y)
{
}

//------------------------------------------------------------------------------------------------------------

function importer(p)
{
    this.tbar;
    this.ledit;
    this.pedit;

    this.curView = "l";

    p.setup = function()
    {
        p.size(800, 680);

        var toolbar = new ToolBar(0, 0, 800, 80);

        toolbar.setup(p);

        var propertyEditor = new PropertyEditor(800, 200);

        propertyEditor.setup(p);

        var levelEdit = new LevelEditor(800, 400);

        levelEdit.tbar = toolbar;

        levelEdit.setup(p, propertyEditor);

        this.tbar  = toolbar;
        this.ledit = levelEdit;
        this.pedit = propertyEditor;

        this.tbar.setAction(function(button) {
            levelEdit.handleButton(button);
        });

        exitHook = function()
        {
            levelEdit.saveExit();
        }

        chromeBackspaceHook = function()
        {
            toolbar.field.keyDown(p, p.BACKSPACE, "");
        }
    }
    
    p.draw = function()
    {       
        p.background(120);

        this.tbar.draw(p);

        p.pushMatrix();
        p.translate(0, this.tbar.b.size.h);
        this.ledit.update(p);     
        p.popMatrix();

        p.pushMatrix();
        p.translate(0, this.tbar.b.size.h + this.ledit.size.h);
        this.pedit.update(p);
        p.popMatrix();
    }

    p.mouseMoved = function()
    {
        if (p.mouseY <= this.tbar.b.size.h)
        {
            if (this.curView != "t")
            {
                this.ledit.mouseLeave(p);
                this.pedit.mouseLeave(p);
                this.tbar.mouseEnter(p);
                this.curView = "t";
            }
            this.tbar.mouseMoved(p, p.mouseX, p.mouseY);
        }
        else if (p.mouseY <= (this.tbar.b.size.h + this.ledit.size.h))
        {
            if (this.curView != "l")
            {
                this.tbar.mouseLeave(p);
                this.pedit.mouseLeave(p);
                this.ledit.mouseEnter(p);
                this.curView = "l";
            }
            this.ledit.mouseMoved(p, p.mouseX, p.mouseY - this.tbar.b.size.h);

        }
        else
        {
            if (this.curView != "p")
            {
                this.tbar.mouseLeave(p);
                this.ledit.mouseLeave(p);
                this.pedit.mouseEnter(p);
                this.curView = "p";
            }
            this.pedit.mouseMoved(p, p.mouseX, p.mouseY - (this.ledit.size.h + this.tbar.b.size.h));
        }
    }

    p.mousePressed = function()
    {
        switch (this.curView)
        {
        case "l":
            this.ledit.mousePressed(p, p.mouseX, p.mouseY - this.tbar.b.size.h);
            break;
        case "p":
            this.pedit.mousePressed(p, p.mouseX, p.mouseY - (this.ledit.size.h + this.tbar.b.size.h));
            break;
        }
    }

    p.mouseReleased = function()
    {
        switch (this.curView)
        {
        case "t":
            this.tbar.mouseReleased(p, p.mouseX, p.mouseY);
            break;
        case "l":
            this.ledit.mouseReleased(p, p.mouseX, p.mouseY - this.tbar.b.size.h);
            break;
        case "p":
            this.pedit.mouseReleased(p, p.mouseX, p.mouseY - (this.ledit.size.h + this.tbar.b.size.h));
            break;
        }
    }

    p.mouseDragged = function()
    {
        switch (this.curView)
        {
        case "l":
            this.ledit.mouseDragged(p, p.mouseX, p.mouseY - this.tbar.b.size.h);
            break;
        case "p":
            this.pedit.mouseDragged(p, p.mouseX, p.mouseY - (this.ledit.size.h + this.tbar.b.size.h));
            break;
        }
    }

    p.keyPressed = function()
    {
        this.tbar.field.keyDown(p, p.keyCode, p.key);
    }

}

//------------------------------------------------------------------------------------------------------------

var chromeBackspaceHook = undefined;

//------------------------------------------------------------------------------------------------------------

$(document).ready(function()
{

    var sketch = new Processing.Sketch();    

    var myCanvas = $("#import_canvas").get(0);

    window.addEventListener('keydown',
                            function(e) {
                                if (e.keyIdentifier == 'U+0008' || e.keyIdentifier == 'Backspace') {
                                    if (e.target == myCanvas) {
                                        e.preventDefault();
                                        if (typeof chromeBackspaceHook === "function")
                                            chromeBackspaceHook();
                                    }
                                }
                            },
                            true);

    loadTemplate();

    
    preload_(sketch);

    
    sketch.imageCache.add("images/next.png");
    sketch.imageCache.add("images/prev.png");
    
    sketch.imageCache.add("images/wall.png");
    sketch.imageCache.add("images/floor.png");
    sketch.imageCache.add("images/undo.png");
    sketch.imageCache.add("images/target.png");
    sketch.imageCache.add("images/goal.png");

    sketch.attachFunction = importer;
    
    new Processing(myCanvas, sketch);


});

