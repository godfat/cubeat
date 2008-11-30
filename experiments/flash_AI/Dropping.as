
class Dropping extends State{
	
    public function Dropping(map: Map){ super(map); }
    public function cycle(that: Square): State{
		that.drop();
        if( that.isNotReached() ){
            return this;
        }
        else{
            that.addjust();
            var result: State = map_.next_state(that);
			if( result == null ) return this;
			else if( result instanceof Waiting ){ return map_.next_state(that); }
			else return result;
        }
    }
	public function toString(){ return "Drop"; }
}
