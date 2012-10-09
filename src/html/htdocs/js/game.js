
function limit(a, min, max)
{
    if (a < min)
        return min;
    if (a > max)
        return max;
    return a;
}

function swap(a, b)
{
    var x = b.x;
    var y = b.y;
    b.x   = a.x;
    b.y   = a.y;
    a.x   = x;
    a.y   = y;
}

//------------------------------------------------------------------------------------------------------------

function displayGameOverNotice(scene)
{
    var lastId = gGameId + HISTORY;
    var tries  = 1 + TRIES;

    var div = sjs.overlay(0, 0, scene.w, scene.h);

    div.className = "wonka";

    var data = 
        "<div class='header'>" + str_game_fail_title + "</div><br>" +
        "<b>" + str_game_fail_under + "</b><br><p>" +
        str_game_fail_body + "<br><p><p><p>" +                         
        "<a class='button gray' href='game.php?id=" + gGameId + "&last=" + HISTORY + "&tries="+tries+"'>" + str_game_play_again + "</a>";

    if (tries > 2 && gUser != 0)
    {
        data += "&nbsp;<a class='button black' href='game.php?id=rand&last=" + lastId + "'>" + str_game_play_random + "</a>";
    }

    div.innerHTML = data;
    scene.dom.appendChild(div);
}

function displayGameWonNotice(scene)
{
    var lastId = gGameId + HISTORY;
    var tries  = TRIES;

    var div = sjs.overlay(0, 0, scene.w, scene.h);
    div.className = "wonka";
    var data =
        "<div class='header'>" + str_game_done_title + "</div><br>" +
        "<b>" + str_game_done_under + "</b><br><p>" +
        str_game_done_body + "<br><p><p><p>" +                         
        "<a class='button gray'  href='game.php?id=" + gGameId + "&last=" + HISTORY + "&tries=" + tries + "'>" + str_game_play_again + "</a>&nbsp;";

    if (gUser != 0)
        data += "<a class='button black' href='game.php?id=rand&last=" + lastId + "'>" + str_game_play_random + "</a>";

    div.innerHTML = data;
    scene.dom.appendChild(div);

}
//------------------------------------------------------------------------------------------------------------

function AudioPlayer()
{
    this.notes = new Array();
    this.sndCollide = undefined;
    this.sndJump = undefined;
    this.sndDeath = undefined;
    this.sndLoop = undefined;
    this.sndWalk = undefined;
    this.sndFly  = undefined;
    this.sndScare = undefined;
    this.sndLift = undefined;

    this.clock = 0;
    this.smin  = 1;
    this.smax  = 8;
    this.next  = 5;
    this.last  = 0;
    this.mute  = false;
}

AudioPlayer.prototype.load = function()
{
    if (soundinfo === undefined)
        return ;

    for (var s in soundinfo)
    {
        var sound = soundinfo[s];

        switch(sound.name)
        {
        case "jump":
            this.sndJump = sound.player;
            break;
        case "collide":
            this.sndCollide = sound.player;
            break;
        case "die":
            this.sndDeath = sound.player;
            break;
        case "loop":
            this.sndLoop = sound.player;
            break;
        case "fly":
            this.sndFly = sound.player;
            break;
        case "walk":
            this.sndWalk = sound.player;
            break;
        case "lift":
            this.sndLift = sound.player;
            break;
        case "scare":
            this.sndScare = sound.player;
            break;
        default:
            this.notes.push(sound.player);
        }
    }

    /*
    if (this.sndLoop !== undefined)
    {
        this.sndLoop.loop = true;
        this.sndLoop.play();
    }
    */

    this.mute = true;
    

}

AudioPlayer.prototype.update = function(dt)
{
    this.clock += dt;

    if (this.clock >= this.next && !this.mute)
    {

        var n = Math.floor(Math.random() * (this.notes.length-1));

        this.notes[this.last].play();
        this.notes[n].play();
        
        var rnd = this.smin + (Math.random()*(this.smax-3));
   
        this.next = this.clock + rnd;
                
        this.last = n;
    }
    

}

AudioPlayer.prototype.win = function()
{
    var n = Math.floor(Math.random() * (this.notes.length-1));

    this.notes[n].play();

}

AudioPlayer.prototype.lift = function()
{
    if (this.sndLift !== undefined)
        this.sndLift.play();
}

AudioPlayer.prototype.scare = function()
{
    if (this.sndScare !== undefined)
        this.sndScare.play();
}

AudioPlayer.prototype.walk = function()
{
    if (this.sndWalk !== undefined)
        this.sndWalk.play();
}

AudioPlayer.prototype.fly = function(state)
{
    return ;
    if(this.sndFly !== undefined)
    {
        this.sndFly.loop = state;
        if (state)
            this.sndFly.play();
        else
            this.sndFly.pause();
    }
}

AudioPlayer.prototype.jump = function()
{
    if (this.sndJump !== undefined)
        this.sndJump.play();
}

AudioPlayer.prototype.die = function()
{
    if (this.sndDeath !== undefined)
        this.sndDeath.play();
}

AudioPlayer.prototype.collision = function()
{
    if (this.sndCollide !== undefined)
        this.sndCollide.play();
}
AudioPlayer.prototype.note = function(i)
{
    this.notes[i].play();
}

//------------------------------------------------------------------------------------------------------------

function Game(width, height)
{
    this.scene    = sjs.Scene({w:width, h:height, autoPause:false});
    this.data     = new Object();
    this.sscale   = "normal";
    this.gravity  = 0.7;
    this.friction = 1;
    this.fullnav  = false;

}

Game.prototype.load = function(id, cb)
{
    var url = "load.php?id=" + id + ((gImportPlayer) ? "&import=" + gImportSource : "");

    var g = this;
    $.getJSON(url, function(data) {
        g.data = data;
        cb(g.scene, data);
    });
}

Game.prototype.getImages = function()
{
    var list = new Array();
    
    list.push(this.data.background.path);

    for(var i in this.data.sprites)
        if (this.data.sprites[i].name != "goal")
            list.push(this.data.sprites[i].path);

    return list;
}

//------------------------------------------------------------------------------------------------------------

function Entity(sprite)
{
    this.s = sprite;
}

Entity.prototype.update = function(env, dt)
{
    this.s.update();
}

Entity.prototype.maxX = function()
{
    return this.s.x + this.s.w;
}

Entity.prototype.maxY = function()
{
    return this.s.y + this.s.h;
}

//------------------------------------------------------------------------------------------------------------

Player.prototype=new Entity();
Player.prototype.constructor=Player;

function Player(sprite)
{
    Entity.prototype.constructor.call(this, sprite);
    
    this.s.lockable = true;
    this.jumping  = false;
    this.collided = false;
}

Player.prototype.update = function(env, dt)
{

    if (this.s.locked)
    {
        Entity.prototype.update.call(this, env, dt);
        return ;
    }

    this.s.yv += env.gravity;
    
    this.s.applyYVelocity();

    if (this.s.collidesWithArray(env.floor))
    {
        this.s.reverseYVelocity();
        this.s.yv = 0;
        this.jumping = false;
    }

    if ((env.input.keyboard.up || env.input.keyboard.space) && !this.jumping)
    {
        this.s.lockable = true;

        if (env.fullnav)
        {
            env.audio.fly(true);
            this.s.yv = (env.forces.jump * env.friction);
        }
        else
        {
            this.s.yv = env.forces.jump * env.friction;
            this.jumping = true;
            env.audio.jump();
        }
    }


    if (env.fullnav && env.input.keyboard.down)
    {
        this.s.lockable = true;
        this.s.yv = env.forces.down * env.friction;
    }

    
    this.s.applyXVelocity();    
    
    if (this.s.collidesWithArray(env.walls) || (this.s.x <= 0 || this.s.x >= env.bg.w - this.s.w))
    {
        this.s.reverseXVelocity();
        this.s.xv = 0;
        this.collided = true;
        env.audio.collision();
    }
    else
        this.collided = false;

    if (env.input.keyboard.right)
    {
        if (env.fullnav)
            env.audio.fly(true);
        else
            env.audio.walk();
        this.s.lockable = true;
        this.s.scale(1, 1);
        this.s.xv = env.forces.forward * env.friction;
    }
    else if (env.input.keyboard.left)
    {
        if (env.fullnav)
            env.audio.fly(true);
        else
            env.audio.walk();
        this.s.lockable = true;
        this.s.scale(-1, 1);
        this.s.xv = env.forces.backward * env.friction;
    }
    else
    {
        if (env.fullnav)
            env.audio.fly(false);
        this.s.xv = 0;
    }

    /*
    if (this.s.collidesWithArray(env.enemy))
    {
        env.audio.collision();
    }
    */

    Entity.prototype.update.call(this, env, dt);
    
}
//------------------------------------------------------------------------------------------------------------

function Controller()
{
    this.xoffset = 0;
    this.yoffset = 0;
    this.list    = sjs.List();
}    

Controller.prototype.add = function(sprite)
{
    this.list = sprite;
}

Controller.prototype.offset = function(x, y)
{
    this.xoffset = x;
    this.yoffset = y;
}

Controller.prototype.update = function(env, dt)
{
    while(s = this.list.iterate())
    {
        s.scroll(this.xoffset, this.yoffset);

        if (s.target != null)
        {
            var dgx = (env.goal.x - s.x);
            var dgy = (env.goal.y - s.y);
            s.move( ((dgx/2)  * dt) * env.friction, 0);
        }
        else
        {
            s.yv += env.gravity;
        
            s.applyYVelocity();
        }

        if (s.collidesWithArray(env.floor))
        {
            s.reverseYVelocity();
            s.yv = 0;
        }

        if (s.collidesWithArray(env.walls))
        {
            s.reverseXVelocity();
            s.xv = env.forces.reverse * env.friction;
            s.yv = env.forces.climb   * env.friction;;
            
        }

        var p = env.player.s;

        var dx =  (p.x - s.x) - this.xoffset;
        var dy =  (p.y - s.y) - this.yoffset;

        var collides = s.collidesWith(p);

        if (s.behavior.cloak)
        {

            if (!collides)
            {
                s.ctimer -= dt;

                if (!s.cloaked && s.ctimer <= 0)
                {
                    s.cloaked = true;
                    s.opacity = 0.05;
                    s.scale(1, 1);
                }
            }
            else
            {
                env.audio.scare();
                s.ctimer  = 1;
                s.cloaked = false;
                s.opacity = 1;
                s.scale(2, 2);
            }
            
        }

        if (dx > 0)
        {
            if (s.xscale > 0)
                s.scale(-1, 1);
        }    
        else
        {
            if (s.xscale < 0)
                s.scale(1, 1);
        }

        if (collides)
        {
            if (s.behavior.push)
            {            
                p.reverseXVelocity();
                p.xv = env.forces.push;
                s.xv = 0;
            }
            
            if (s.behavior.block)
            {
                p.xv = 0;
                s.xv = 0;
            }

            if (s.behavior.help)
            {
                if (!p.locked && p.lockable)
                {
                    p.locked = true;
                    p.lockable = false;
                    s.target = p;
                    s.ltime  = 0;
                    p.opacity = 0.2;
                    env.audio.lift();
                }
                
            }

        }
        else
        {
            if (s.behavior.follow)
            {
                if (dx > 0)
                    s.xv =  env.forces.intercept * env.friction;
                else
                    s.xv = -env.forces.intercept * env.friction;

                if (env.fullnav)
                {
                    if (dy > 0)
                        s.yv =  env.friction;
                    else
                        s.yv = - env.friction;
                }
            }
        }    

        if (s.target == p && p.locked)
        {
            s.ltime += dt;
            
            if (s.y > 50)
            {
                s.move(0, -20 * env.friction);
            }
            
            p.position(s.x + this.xoffset, s.y-s.w);
            
            if (s.ltime >= 2)
            {
                s.behavior.follow = false;
                s.xv = -2 * env.friction;

                p.opacity = 1.0;
                p.locked = false;
                s.target = null;
            }
        }

        
        if (s.behavior.hide)
        {

        }


        s.applyXVelocity();


        s.update(env, dt);
    }

}

Controller.prototype.move_intercept = function(s, dx, dy, env, dt)
{
    var p = env.player.s;




}

Controller.prototype.move_flee = function(s, env, dt)
{
    
}

//------------------------------------------------------------------------------------------------------------

$(document).ready(function()
{
    loadSounds();

    var game = new Game(WIDTH, HEIGHT);

    var audio = new AudioPlayer();

    audio.load();

    var gtitle = $("#gname").get(0);

    game.load(gGameId, function(scene, assets)
    {
        gtitle.innerHTML = assets.name;
        
        scene.loadImages(game.getImages(), function() 
        {

            var blayer;
            var flayer;
            var background;
            var player;
            var enemy;
            var goal;
            var floor;
            var walls;
            var input;
            var setup;
            var env;

            var forces;
            var debug = 0;

            blayer     = scene.Layer("background", {useCanvas:true, autoClear:false});
            flayer     = scene.Layer("foreground", {useCanvas:true, autoClear:true});
            background = scene.Sprite(assets.background.path, blayer);
            

            setup = assets.background.properties;

            for (var i in setup)
            {
                var p = setup[i];

                if (!p.enabled)
                    continue;
                
                log("enable: " + p.name);

                switch(p.name)
                {
                case "highgravity": // Sirap
                    game.friction = 0.2;
                    game.gravity  = 0.001;                    
                    game.fullnav  = true; 
                    forces = { jump:-5, forward:4, backward:-4, down:2, climb: -5, reverse: -5, intercept: 2, push: -10};
                    break;
                case "normal":
                    game.friction = 1;
                    game.gravity  = 0.7;
                    forces = { jump:-15, forward:4, backward:-4, down:10, climb: -5, reverse: -5, intercept: 2, push: -10};
                    break;
                case "friction": { // vatten
                    game.friction = 0.5;
                    game.gravity  = 0.01;
                    game.fullnav  = true;
                    forces = { jump:-2, forward:4, backward:-4, down:2, climb: -2, reverse: -5, intercept: 2, push: -10};
                    break;
                }
                case "lowgravity": // flyg
                    game.gravity  = 0;
                    game.friction = 0.7;
                    game.fullnav  = true;
                    forces = { jump:-2, forward:4, backward:-4, down:2, climb: -2, reverse: -4, intercept: 2, push: -10};
                    break;
                }
            }

            for (var i in setup)
            {
                var p = setup[i];

                if (!p.enabled)
                    continue;
                
                log("enable: " + p.name);

                switch(p.name)
                {
                case "microsized": {
                    game.sscale = "tiny";
                    //forces = { jump:-15, forward:4, backward:-4, down:10, climb: -5, reverse: -5, intercept: 2, push: -10}
                    break;
                }
                case "mediumsized": {                    
                    game.sscale = "small";
                    //forces = { jump:-15, forward:4, backward:-4, down:10, climb: -5, reverse: -5, intercept: 2, push: -10}
                    break;
                }
                case "normalsized": {
                    game.sscale = "normal";
                    //forces = { jump:-15, forward:4, backward:-4, down:10, climb: -5, reverse: -5, intercept: 2, push: -10}
                    break;
                }
                }
            }
            
            enemy = sjs.List();
            floor = sjs.List();
            walls = sjs.List();
            ctrl  = new Controller();

            var clx = 0;
            var cly = 0;
                        
            var slx = (background.w / EDITOR.width);
            var sly = (background.h / EDITOR.height);

            for (var i in assets.floor)
            {
                var fp = assets.floor[i];
                var p1 = {x:(fp[0].x * slx) + clx, y:(fp[0].y * sly) + cly};                
                var p2 = {x:(fp[1].x * slx) + clx,  y:(fp[1].y * sly) + cly};

                if (p2.x < p1.x)
                    swap(p1, p2);

                var so = {layer:blayer, x:p1.x, y:p1.y, size:[p2.x-p1.x, 10], opacity:0, color:"#FF00FF"};
                var ns = scene.Sprite(false, so);
                floor.add(ns);
            }

            for (var i in assets.walls)
            {
                var fp = assets.walls[i];
                var p1 = {x:(fp[0].x * slx) + clx, y:(fp[0].y * sly) + cly};
                var p2 = {x:(fp[1].x * slx) + clx, y:(fp[1].y * sly) + cly};

                if (p2.y < p1.y)
                    swap(p1, p2);

                var so = {layer:blayer, x:p1.x, y:p1.y, size:[10, p2.y-p1.y], opacity:0, color:"#00FFFF"};
                var ns = scene.Sprite(false, so);
                walls.add(ns);
            }

            for (var i in assets.sprites)
            {
                var s = assets.sprites[i];

                var res = s.res[game.sscale];
                
                var x = 0, y = 0;

                if (s.spawn)
                {
                    x = s.spnx  * slx;
                    y = s.spny  * sly;
                }

                if (res)
                {
                    x-=(res.width/2);
                    y-=(res.height/2);
                }

                var opt = {layer:flayer, x: x, y: y};

                switch(s.name)
                {
                case "main":
                    player = new Player(scene.Sprite(res.path, opt));
                    break;
                case "goal":
                    opt.size = [50, 50];
                    opt.x -= 25;
                    opt.y -= 25;
                    //opt.color = "#FFFF00";
                    opt.opacity = 0.2;
                    goal = scene.Sprite(false, opt);
                    break;
                default:
                    var ns = scene.Sprite(res.path, opt);
                    ns.ctimer = 0;
                    ns.behavior = new Object();
                    for (var k in s.properties)
                        ns.behavior[s.properties[k].name] = s.properties[k].enabled;

                    enemy.add(ns);
                    break;
                }                
                
            }

            
            
            ctrl.add(enemy);

            input = scene.Input();

            env = {
                gravity:game.gravity,
                friction:game.friction,
                scene:scene,
                bg:background,
                input:input, 
                player:player, 
                enemy:enemy,         
                goal: goal,
                floor:floor, 
                walls:walls,
                audio:audio,
                forces: forces,
                fullnav:game.fullnav
            };

            var trx=0, dtx=0, limx=0, lw=scene.w/4;

            function paint()
            {
                
                var dt = (ticker.lastTicksElapsed * (1.0/ticker.tickDuration));

                player.update(env, dt);

                if (player.s.y > scene.h)
                {
                    audio.die();
                    ticker.pause();
                    displayGameOverNotice(scene);
                    return ;
                }
                
                if (player.s.collidesWith(goal))
                {
                    audio.win();
                    ticker.pause();
                    displayGameWonNotice(scene);
                    return ;
                }


                if (!player.collided)
                {
                    dtx = ((player.maxX() - lw) - trx);
                    
                    trx += (dtx * ((game.friction * 10)* dt));

                    trx = limit(trx, 0, scene.w/2);
                }

                background.scroll(-trx, 0);
               
                background.update();

                ctrl.offset(-trx, 0);

                ctrl.update(env, dt);


                while(el = floor.iterate())
                {
                    el.scroll(-trx, 0);
                    el.update();
                }

                while(el = walls.iterate())
                {
                    el.scroll(-trx, 0);
                    el.update();
                }

                goal.scroll(-trx, 0);
                goal.update();


            }

            var ticker = scene.Ticker(30, paint);
            ticker.run();
        });
        
    });




});
