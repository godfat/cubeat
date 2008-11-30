
class Dying extends State{
	
    public function Dying(map: Map){ super(map); }
	public function cycle(that:Square): State{
        if(that.body._alpha > 0){
            that.body._alpha -= map_.Decay;
            return this;
        }
        else{
			that.clear_tag();  //added: important when the square dies we need to clear one tag
            return null;
        }
    }
	public function toString(){ return "Die"; }
}
