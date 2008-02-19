
class Ai{
	public function Ai(game: Game, map: Map){
		game_ = game;
		map_ = map;
		trace("Hi,I am Ai!");
	}
	
	public function travelMap(map: Map)
	{
		var brain_map: Map = set_brain_map(map);
		var check_data: Array = brain_map.getData();
		del_a_block(brain_map, 1, 7);
		var chain_arr: Array = chain_checker(brain_map);
		//trace(chain_arr.length);
		/*for(var ty = brain_map.Height-1; ty >= 1; --ty){
			for(var tx = brain_map.Width-1; tx >= 0; --tx){
				if(brain_map.lookup(tx,ty) != null && brain_map.lookup(tx,ty).state instanceof Waiting){
					del_a_block(brain_map, 1, 7);
				}
			}
		}*/
	}
	
	private function set_brain_map (map: Map): Map{
		var clone_map: Map = map.clone();
		trace("map clone");
		trace_map_state(clone_map);
		return clone_map;
	}
	
	private function chain_counter(map: Map){
		var temp_map = map;
		var chain_arr: Array = new Array();
		
	}
	
	private function chain_checker(map: Map): Array{
		var chain_arr: Array = new Array();
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.make_row(map.lookup(w,h)) != null || map.make_column(map.lookup(w,h)) != null ){
					trace("check(" + w + "," + h + ")");
					trace("MR : " + map.make_row( map.lookup(w,h) ) );
					trace("MC : " + map.make_column( map.lookup(w,h) ) );
				}
				//chain_arr.push( map.make_row( map.lookup(w,h) ) );
				//chain_arr.push( map.make_column( map.lookup(w,h) ) );
				//trace("now push : " + chain_arr + "Length = " + chain_arr.length);
			}
		}
		return chain_arr;
	}
	
	//private function 
	
	private function del_a_block(map: Map, x: Number, y: Number)
	{
		var temp_map = map;
		var del_x = x;
		var del_y = y;
		trace("delete(" + del_x + "," + del_y + ")");
		temp_map[del_y][del_x].suicide();
		temp_map.delData(del_x, del_y);
		for(var move_y = del_y; move_y >=2; --move_y){
			if(temp_map.lookup(del_x,move_y-1) != null && temp_map.lookup(del_x,move_y-1).state instanceof Waiting){
				temp_map.setup(del_x, move_y, temp_map.lookup(del_x,move_y-1));
				temp_map[move_y-1][del_x].suicide();
				temp_map.delData(del_x, move_y-1);
				trace("move (" + del_x + "," + (move_y-1) + ") to (" + del_x + "," + move_y + ")");
			}
		}
	}
	
	private function trace_map_state(map: Map){
		var temp_arr: Array = map.getData();
		var state_arr: Array = new Array(map.Height);
        for(var i = 0; i < map.Height; ++i){
			state_arr[i] = new Array(map.Width);
		}
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				//state_arr[h][w] = new Array(2);
				//state_arr[h][w][0] = map.lookup(w,h).state;
				//state_arr[h][w][1] = map.lookup(w,h).rgb;
				state_arr[h][w] = map.lookup(w,h).state;
			}
		}
		trace("old:");
		for(var a = 0; a < map.Height; ++a){
			trace(state_arr[a]);
		}
	}
	
	private var game_: Game;
	private var map_: Map;
}