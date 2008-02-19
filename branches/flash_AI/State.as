
class State{

    private function State(map: Map){ map_ = map; }
	public function cycle(that: Square): State{
        trace("this is an abstract(pure virtual) function. if you read this, then there should be something wrong in your code.");
        return undefined;
    }

    private var map_: Map;
}
