
class Waiting extends State{

    public function Waiting(map: Map){ super(map); delay_ = 0; }
	public function cycle(that:Square): State{
        ++delay_;
        var result:State = map_.next_state(that);
		if( result == null ) return this;
		else return result;
    }
    public function needCheck(): Boolean{ return !checked_ && delay_ >= map_.Delay; }
    public function checked(): Void{ checked_ = true; }
	public function uncheck(): Void{ checked_ = false; }
	public function state_checked(): Boolean{ return checked_; }  //added
	public function toString(){ return "Waiting"; }
    private var delay_: Number;
    private var checked_: Boolean = false;
}
