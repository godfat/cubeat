
import flash.geom.ColorTransform;
import flash.geom.Transform;

class Item
{
	public function Item(game: Game, type: Number) {
 		game_ = game;
		type_ = type;
        var depth: Number = game_.next_depth();
        body_ = game_.body.attachMovie("Square", "Item"+depth, depth, {_x: type_ == 1 ? -50 : 650, _y: 0});
        body_.self = this;
		type_ == 1 ? speed_ = 6.5 : speed_ = -6.5;
		type_ == 1 ? destination_ = 650 : destination_ = -50;
		
		trans_ = new Transform(body_.inner);
        color_ = new ColorTransform();
		
		color_.redOffset = 200;
		color_.greenOffset = 200;
		color_.blueOffset = 200;
        
		trans_.colorTransform = color_;
		
		debug_tag = body_.createTextField("tag"+depth, game_.next_depth(), 2, 18, 60, 18);
		gun_type_ = Math.floor( Math.random()*4 );
		switch ( gun_type_ ) {
			case 0: debug_tag.text = "Heavy P."; break;
			case 1: debug_tag.text = "Mach.Gun"; break;
			case 2: debug_tag.text = "H.Mach.";  break;
			case 3: debug_tag.text = "Rocket";   break;
		}
	}
	
	public function i_am_hit(): Void {
		this.suicide(); //then we have no "Dying" effect here ......... I forgot about this.
	}
	
	public function cycle(): Boolean {
		body_._x += speed_;
		
		if( reached_dest() ) {
			this.suicide();
			return false;
		}
		return true;
	}
	
	private function reached_dest(): Boolean {
		if( type_ == 1 ) {
			return body_._x >= destination_;
		}
		else {
			return body_._x <= destination_;
		}
	}
	
	public function suicide(): Void {
        body_.removeMovieClip();
        delete this;
    }
	
	public function item_type(): Number { return gun_type_; }
	public function get body(): MovieClip { return body_; }
		
	private var destination_: Number;
	private var speed_: Number;
    private var body_: MovieClip;
	private var game_: Game;
    private var trans_: Transform;
    private var color_: ColorTransform;
	private var debug_tag: TextField;
	private var gun_type_: Number;
	private var type_: Number;
	
    // dummy
    private var self: Item;
}