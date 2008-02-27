
class Ai{
	public function Ai(game: Game, map: Map){
		game_ = game;
		map_ = map;
		max_grace = 0;
		best_first_step_x = -1;
		best_first_step_y = -1;
		best_second_step_x = -1;
		best_second_step_y = -1;
		test_shoot = false;
		trace("Hi,I am Ai!");
	}
	public function starter(){
		trace("travel start");
	}
	
	public function travelMap(map: Map){
		//trace("travel start");
		reset_best_step();
		var brain_map: Map = set_brain_map(map);
		trace("shoot(1,7) = " + test_shoot);
		if(test_shoot == true){
			del_block(brain_map, 4, 10, true);
			trace("map_after del(1,7) >>");
			test_shoot = false;
		}else{
			trace("old map >>");
		}
		trace_map_state(brain_map);
		trace_map_rgb(brain_map);
		//chain_counter(brain_map, 4, 10);
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.lookup(w,h)!= null){
					var temp_map: Map = set_brain_map(map);
					del_block(temp_map, w, h, true);
					chain_counter(temp_map, w, h);
				}	
			}
		}
		/*for(var chain_Num = 0; chain_Num < chain_arr.length; ++chain_Num){
			trace("chain-" + chain_Num + " : " + chain_arr[chain_Num]);
			for(var cube_Num = 0; cube_Num < chain_arr[chain_Num].length; ++cube_Num){
				trace("cube(" + chain_Num + "-" + cube_Num + ")"
					  + " at (" + chain_arr[chain_Num][cube_Num].x + "," + chain_arr[chain_Num][cube_Num].y + ")"
					  + " color : " + chain_arr[chain_Num][cube_Num].rgb);
			}			
		}*/
		trace("Best step(" + best_second_step_x + "," + best_second_step_y + ")");
	}
	//clone true map to think
	private function set_brain_map (true_map: Map): Map{
		var clone_map: Map = true_map.clone();
		trace("map clone");
		trace("clone Map >>");
		trace_map_state(clone_map);
		trace_map_rgb(clone_map);
		return clone_map;
	}
	//count chain
	private function chain_counter(brain_map: Map, cc_x: Number, cc_y: Number){
		var square_count: Array = new Array();
		var chain_count: Number = 0;
		var get_grace: Number = 0;
		trace(chain_checker(brain_map));
		while(chain_checker(brain_map) != null && chain_count < 10){
			trace("del chain(len=" + chain_checker(brain_map).length + ") : "+ chain_checker(brain_map) );
			++chain_count;
			square_count.push(chain_checker(brain_map).length);
			chain_del(brain_map, chain_checker(brain_map));
			//trace_map_state(brain_map);
			//trace_map_rgb(brain_map);
			trace("chain_count = " + chain_count);
			trace("square_count = " + square_count[chain_count-1]);
			trace("next chain : "  + chain_checker(brain_map));
		}
		//(del_square_in_this_chain-3)*this_chain_Num+2^(this_chain_Num-2)
		for(var i = 0; i < square_count.length; ++i){
			get_grace += (square_count[i]-3)*i+1;
			if( i == 0 ){
				get_grace += (square_count[i]-3)*(i+1);
			}else{
				get_grace += (square_count[i]-3)*(i+1) + Math.pow(2, i-1);
			}	
		}	
		if(get_grace > this.max_grace){
			this.max_grace = get_grace;
			this.best_second_step_x = cc_x;
			this.best_second_step_y = cc_y;
		}
		trace("Final_chain_count = " + chain_count);
		trace("All_square_count  = " + square_count);
		trace("this grace:" + get_grace);
		trace_map_state(brain_map);
		trace_map_rgb(brain_map);
	}
	//find chain
	private function chain_checker(brain_map: Map): Array{
		var temp_arr: Array = new Array();
		var push_arr: Array = new Array();
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.make_row(brain_map.lookup(w,h)).length >= 2){
					push_arr = brain_map.make_row( brain_map.lookup(w,h) );
					push_arr.push( brain_map.lookup(w,h) );
					//trace("find row from (" + w + "," + h + ")");
					//trace("MR(len=" + push_arr.length + ") : "+ push_arr);
					for(var push_row = 0; push_row < push_arr.length; ++push_row){
						if(push_arr[push_row].cycled == false){
							push_arr[push_row].cycled = true;
							temp_arr.push(push_arr[push_row]);
						}
					}
				}
				if(brain_map.make_column(brain_map.lookup(w,h)).length >= 2){
					push_arr = brain_map.make_column( brain_map.lookup(w,h) );
					push_arr.push( brain_map.lookup(w,h) );
					//trace("find col from (" + w + "," + h + ")");
					//trace("MC(len=" + push_arr.length + ") : "+ push_arr);
					for(var push_col = 0; push_col < push_arr.length; ++push_col){
						if(push_arr[push_col].cycled == false){
							push_arr[push_col].cycled = true;
							temp_arr.push(push_arr[push_col]);
						}
					}
				}
			}
		}
		for(var re_cy = 0; re_cy < temp_arr.length; ++re_cy){
			temp_arr[re_cy].cycled = false;
		}
		//trace("All_pushed_square(len=" + temp_arr.length + ") : "+ temp_arr );
		if(temp_arr.length == 0){
			return null;
		}else{
			return temp_arr;
		}
	}
	private function chain_del(brain_map: Map, del_arr: Array){
		trace("start chain_del");
		//var del_arr: Array = new Array();
		//del_arr = chain_arr; 
		for(var cube_Num = 0; cube_Num < del_arr.length; ++cube_Num){
			del_block(brain_map, del_arr[cube_Num].x, del_arr[cube_Num].y, false);
			trace("del(" + del_arr.length + "-" + cube_Num + ") = " + del_arr);
		}
		//trace("del_arr = " + del_arr);
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.lookup(w, h) == null){
					move_block(brain_map, w, h);
					//trace("move(" + tx + "," + ty + ")");
				}
			}
		}
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
	
	private function reset_best_step(){
		this.best_first_step_x = -1;
		this.best_first_step_y = -1;
		this.best_second_step_x = -1;
		this.best_second_step_y = -1;
		this.max_grace = 0;
	}
	
	private var best_first_step_x: Number;
	private var best_first_step_y: Number;
	private var best_second_step_x: Number;
	private var best_second_step_y: Number;
	private var max_grace: Number;
	private var game_: Game;
	private var map_: Map;
	private var test_shoot: Boolean;
}