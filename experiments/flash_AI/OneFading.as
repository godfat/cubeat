
class OneFading{

    public function OneFading(map: Map, data: Array){
        map_ = map;
        data_ = data;
    }

    public function size(): Number{ return data_.length; }
    public function fade(): Void{
        var tag: Tag = get_tag();
        for(var i = 0; i < data_.length; ++i ){
            // mark all include self
            make_tags(data_[i], tag);
            data_[i].state = map_.make_Dying();
			data_[i].cycled = true;
        }
    }

	//changed a lot here
    private function get_tag(): Tag{
        var tag: Tag = null;
        var i: Number = 0;
		
		var is_chain = false;
		
		//this for loop will get the first tag it finds in this column or row
		for(; i< data_.length; ++i) {       
			if( data_[i].tag != null && data_[i].tag != undefined ) {
                tag = data_[i].tag;
                break;
            }
		}
		
		//if we can't find any useful tag here, we'll create one
		if( tag == null ) {
			tag = new Tag( map_.num_of_chains() );
			tag.add_num_of_combo();
			map_.push_tag( tag );
			
			trace( "map" + map_.id() + ", " + map_.num_of_chains() + "th tag created." );
		}
				
	    //and add combo to it
		tag.add_combo( data_.length );
				
		return tag;
    }
	
    private function make_tags(square: Square, tag: Tag): Void{
        for(var x = square.x, y = square.y;
            map_.lookup(x, y) != null &&
            map_.lookup(x, y).state instanceof Waiting &&  //added: important
			map_.lookup(x, y).tag == null;  //added: important
            --y)
        {
            map_.lookup(x, y).tag = tag;
			tag.inc_tag();  //added: important
        }
    }
    private var data_: Array;
    private var map_: Map;
}
