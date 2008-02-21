
class Ai{
	public function Ai(game: Game, map: Map){
		game_ = game;
		map_ = map;
		test_shoot = true;
		trace("Hi,I am Ai!");
	}
	
	public function travelMap(map: Map){
		var brain_map: Map = set_brain_map(map);
		trace(test_shoot);
		if(test_shoot == true){
			del_block(brain_map, 1, 7, true);
			trace("map_after del(1,7) >>");
			test_shoot = false;
		}else{
			trace("old map >>");
		}
		trace_map_state(brain_map);
		trace_map_rgb(brain_map);
		var chain_arr: Array = chain_checker(brain_map);
		//var chain_arr: Array = new Array;
		for(var chain_Num = 0; chain_Num < chain_arr.length; ++chain_Num){
			trace("chain-" + chain_Num + " : " + chain_arr[chain_Num]);
			for(var cube_Num = 0; cube_Num < chain_arr[chain_Num].length; ++cube_Num){
				trace("cube(" + chain_Num + "-" + cube_Num + ")"
					  + " at (" + chain_arr[chain_Num][cube_Num].x + "," + chain_arr[chain_Num][cube_Num].y + ")"
					  + " color : " + chain_arr[chain_Num][cube_Num].rgb);
			}			
		}
		for(var chain_Num = 0; chain_Num < chain_arr.length; ++chain_Num){
			for(var cube_Num = 0; cube_Num < chain_arr[chain_Num].length; ++cube_Num){
				var tx = chain_arr[chain_Num][cube_Num].x;
				var ty = chain_arr[chain_Num][cube_Num].y;
				del_block(brain_map, tx, ty, false);
			}			
		}
		//chain_arr = null;
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.lookup(w, h) == null){
					move_block(brain_map, w, h);
				}
			}
		}
		trace("map_after del chain >>");
		trace_map_state(brain_map);
		trace_map_rgb(brain_map);
		trace(chain_arr + " - " + chain_arr.length);
	}
	
	private function set_brain_map (true_map: Map): Map{
		var clone_map: Map = true_map.clone();
		trace("map clone");
		trace("clone Map >>");
		trace_map_state(clone_map);
		trace_map_rgb(clone_map);
		return clone_map;
	}
	
	private function chain_counter(brain_map: Map){
		//var temp_map = brain_map;
		//var temp_arr: Array = new Array();
		
	}
	
	private function chain_checker(brain_map: Map): Array{
		var temp_arr: Array = new Array();
		var push_arr: Array = new Array();
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.make_row(brain_map.lookup(w,h)).length >= 2){
					push_arr = brain_map.make_row( brain_map.lookup(w,h) );
					push_arr.push( brain_map.lookup(w,h) );
					trace("push : (" + w + "," + h + ")");
					trace("MR(length=" + push_arr.length + ") : "+ push_arr);
					temp_arr.push(push_arr);
				}
				if(brain_map.make_column(brain_map.lookup(w,h)).length >= 2){
					push_arr = brain_map.make_column( brain_map.lookup(w,h) );
					push_arr.push( brain_map.lookup(w,h) );
					trace("push : (" + w + "," + h + ")");
					trace("MC(length=" + push_arr.length + ") : "+ push_arr);
					temp_arr.push(push_arr);
				}
			}
		}
		trace( temp_arr );
		//push_arr = null;
		return temp_arr;
	}
	
	private function del_block(brain_map: Map, x: Number, y: Number, need_move: Boolean){
		var temp_map: Map = brain_map;
		var del_x = x;
		var del_y = y;
		trace("delete(" + del_x + "," + del_y + ")");
		temp_map[del_y][del_x].suicide();
		temp_map.delData(del_x, del_y);
		if(need_move == true){
			move_block(temp_map, del_x, del_y);
		}
	}
	
	private function move_block(brain_map: Map, x: Number, y: Number){
		var temp_map: Map = brain_map;
		var locx = x;
		var locy = y;
		for(var target_y = locy-1;
			target_y > 0 &&
			temp_map.lookup(locx,locy) == null;
			--target_y){
			if(temp_map.lookup(locx,target_y) != null && temp_map.lookup(locx,target_y).state instanceof Waiting){
				temp_map.setup(locx, locy, temp_map.lookup(locx, target_y));
				temp_map.lookup(locx, locy).y = locy;
				trace("move (" + locx + "," + target_y + ") to (" + locx + "," + locy + ")");
				del_block(temp_map, locx, target_y, true);
			}
		}
	}
	
	private function trace_map_state(brain_map: Map){
		var state_arr: Array = new Array(brain_map.Height);
        for(var i = 0; i < brain_map.Height; ++i){
			state_arr[i] = new Array(brain_map.Width);
		}
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if( brain_map.lookup(w,h).state instanceof Waiting )
				    state_arr[h][w] = "Wa";
				else if ( brain_map.lookup(w,h).state instanceof Dropping )
				    state_arr[h][w] = "Dr";
				else if ( brain_map.lookup(w,h).state instanceof Dying )
				    state_arr[h][w] = "Dy";
				else
				    state_arr[h][w] = "--";
			}
		}
		trace("map_state:");
		for(var a = 0; a < brain_map.Height; ++a){
			trace(state_arr[a]);
		}
	}
	
	private function trace_map_rgb(brain_map: Map){
		var rgb_arr: Array = new Array(brain_map.Height);
        for(var i = 0; i < brain_map.Height; ++i){
			rgb_arr[i] = new Array(brain_map.Width);
		}
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if( brain_map.lookup(w,h).rgb == 255 << 16 )
				    rgb_arr[h][w] = "Re";
				else if ( brain_map.lookup(w,h).rgb == 255 << 8 )
				    rgb_arr[h][w] = "Gr";
				else if ( brain_map.lookup(w,h).rgb == 255 )
				    rgb_arr[h][w] = "Bl";
				else if ( brain_map.lookup(w,h).rgb == (255<<16) + (255<<8) )
				    rgb_arr[h][w] = "Ye";
				else
				    rgb_arr[h][w] = "--";
			}
		}
		trace("map_RGB:");
		for(var a = 0; a < brain_map.Height; ++a){
			trace(rgb_arr[a]);
		}
	}
	
	private var game_: Game;
	private var map_: Map;
	private var test_shoot: Boolean;
}