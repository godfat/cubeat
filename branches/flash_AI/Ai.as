
class Ai{
	public function Ai(game: Game, map: Map){
		game_ = game;
		map_ = map;
		travel_limit = 3;//make AI only travel the last (11-limit) Row of Map
		best_amass_point = [0,0,0];
		best_amass_step_x = [-1,-1,-1];
		best_amass_step_y = [-1,-1,-1];
		best_chain_point = [0,0,0];
		best_chain_step_x = [-1,-1,-1];
		best_chain_step_y = [-1,-1,-1];
		best_chain_cubes = new Array(3);
		shooted = false;
		chaining = false;
		clear_garbage_x = new Array();
		clear_garbage_y = new Array();
		trace("Hi,I am Ai!");
	}
	
	public function full_process(map: Map){
		/*for(var h = map.Height-1; h > this.travel_limit; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w, h).state instanceof Dropping && map.lookup(w, h).first_drop == false){
					this.shooted = true;
					break;
				}else{
					this.shooted = false;
				}
			}
			if(this.shooted != true){
				break;
			}
		}
		//combo_rank_update(map);
		if(this.shooted != true){
			bk_gbg_travel(map);
			if(count_square_Num(map) >= 30 || this.map_tops[map.Width-1+2] <= 2){
				combo_travel(map);
			}
			if(count_square_Num(map) >= 10 && this.best_chain_step_x == -1){
				amass_travel(map);
			}
		}*/
		trace_map_rgb(map);
		var del_arr: Array = too_high_travel(map);
		for(var i = 0; i<del_arr.length; ++i){
			if(del_arr[i]!=null){
				for(var j=0;j<del_arr[i].length; ++j){
					del_arr[i][j].i_am_hit(1);
				}
			}
		}
		best_amass_point = [0,0,0];
		best_amass_step_x = [-1,-1,-1];
		best_amass_step_y = [-1,-1,-1];
		best_chain_point = [0,0,0];
		best_chain_step_x = [-1,-1,-1];
		best_chain_step_y = [-1,-1,-1];
	}
	//clone true map to think
	private function set_brain_map (map: Map): Map{
		var clone_map: Map = map.clone();
		return clone_map;
	}
	
	private function count_square_Num(map: Map): Number{
		var count:Number = 0;
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if( map.lookup(w,h).state instanceof Waiting ){
					++count;
				}
			}
		}
		return count;
	}
		
	private function top_update(map: Map): Array{
		var map_tops = new Array(map.Width+2);
		for(var i = 0; i < map.Width+2; i++){
			map_tops[i] = 0;
		}
		//map_tops[0~5] = every top in map, map_tops[6] = average, map_tops[7] = highest
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0; --w){
				if( map.lookup(w,h).state instanceof Waiting){
					map_tops[w] = map.Height-h;
				}
			}
		}
		for(var w = map.Width-1; w >= 0; --w){
			map_tops[map.Width-1+1] += map_tops[w];
			if(map_tops[w] > map_tops[map.Width-1+2]){
				map_tops[map.Width-1+2] = map_tops[w];
			}
		}
		map_tops[map.Width-1+1] = int(map_tops[map.Width-1+1]/map.Width);
		trace("map_tops:" + map_tops);
		return map_tops;
	}
		
	private function too_high_travel(map: Map): Array{
		var map_tops: Array = top_update(map);//get high of each column
		var del_list: Array = new Array(map.Width);//to save cubes that need delete
		for(var i = 0; i < del_list.length; ++i){
			del_list[i] = null;
		}
		for(var w = map.Width-1; w > 0; --w){
			//do if this column higher-2 > average high
			trace(map_tops[w] - map_tops[map.Width-1+1]);
			if(map_tops[w] - map_tops[map.Width-1+1] >= 2){
				//travel this column
				trace("column(" + w + ")too high");
				del_list[w] = column_travel(map, w);
			}
		}
		for(var j = 0; j < del_list.length; ++j){
			if(del_list[j]!=null){
				return del_list;
			}
		}
		return null;
	}
	private function column_travel(map: Map, w: Number): Array{
		var del_list: Array = new Array;//to save cubes that need delete
		trace("column_travel start!");
		for(var h = map.Height-1; h > 0; --h){
			if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
				trace("check(" + h + ")>>" + map.make_column(map.lookup(w,h)).length);
				//find 2-contact at this column
				if(map.make_column(map.lookup(w,h)).length >= 1){
					//check which one is higher than the other
					//map.make_column(map.lookup(w,h))[0] always higher(<) than h
					trace("find 2-contact at(" + w + "," + h + ")");
					var del_list_up: Array = new Array;
					var del_list_down: Array = new Array;
					for(var ph = 1; h + ph < 11 && h - 1 - ph > 0 && ph < 3; ++ph){
						if(map.lookup(w,h + ph).rgb == map.lookup(w,h).rgb && map.lookup(w,h + ph).state instanceof Waiting){
							trace("column_travel_dw find!");
							del_list = del_list_down;
							break;
						}else{
							trace("push_dw: " + w + "," + (h + ph));
							del_list_down.push(map.lookup(w,h + ph));
						}
						if(map.lookup(w,h - 1 - ph).rgb == map.lookup(w,h).rgb && map.lookup(w,h - 1 - ph).state instanceof Waiting){
							trace("column_travel_up find!");
							del_list = del_list_up;
							break;
						}else{
							trace("push_up: " + w + "," + (h - 1 - ph));
							del_list_up.push(map.lookup(w,h - 1 - ph));
						}
					}
					if(del_list.length != 0){
						break;
					}
					--h;
				}
			}
		}
		if(del_list.length != 0){
			return del_list;
		}else{
			return null;
		}
	}
	private function bk_gbg_travel(map: Map){
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0; --w){
				if(map.lookup(w,h).state instanceof Waiting && map.lookup(w,h).if_broken() == true){
					trace("shoot broken(" + w + "," + h +")");
					map.lookup(w,h).i_am_hit(1);
				}else if(map.lookup(w,h).state instanceof Waiting && map.lookup(w,h).if_garbage() == true){
					trace("shoot garbage(" + w + "," + h +")");
					map.lookup(w,h).i_am_hit(1);
				}
			}
		}
	}
	
	private function combo_rank_update(map: Map){
		if(true){
			var temp_point: Number = 0;
			this.best_chain_point = [0,0,0];
			this.best_chain_step_x = [-1,-1,-1];
			this.best_chain_step_y = [-1,-1,-1];
			this.best_chain_cubes = [null,null,null];
			for(var h = map.Height-1; h > this.travel_limit; --h){
				for(var w = map.Width-1; w >= 0 ; --w){
					if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
						var temp_map: Map = set_brain_map(map);
						del_block(temp_map, w, h, true);
						temp_point = combo_counter(temp_map);
						for(var i = this.best_chain_point.length; i >=0; --i){
							if(temp_point > this.best_chain_point[i]){
								for(var j = 0; j < i; ++j){
									this.best_chain_point[j] = this.best_chain_point[j+1];
									this.best_chain_step_x[j] = this.best_chain_step_x[j+1];
									this.best_chain_step_y[j] = this.best_chain_step_y[j+1];
									this.best_chain_cubes[j] = this.best_chain_cubes[j+1];
								}
								this.best_chain_point[i] = temp_point;
								this.best_chain_cubes[i] = map.lookup(w,h);
								this.best_chain_step_x[i] = map.lookup(w,h).x;
								this.best_chain_step_y[i] = map.lookup(w,h).y;
								break;
							}
						}
						delete temp_map;
					}
				}
			}
			for(var l = 0; l < this.best_chain_point.length; ++l){
				trace("Best combo<" + best_chain_point[l] + ">(" + best_chain_step_x[l] + "," + best_chain_step_y[l] + ")");
			}
		}
	}
	
	private function combo_travel(map: Map){
		if(true){
			var combo_point = 0;
			for(var h = map.Height-1; h > this.travel_limit; --h){
				for(var w = map.Width-1; w >= 0 ; --w){
					if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
						var temp_map: Map = set_brain_map(map);
						del_block(temp_map, w, h, true);
						combo_point = combo_counter(temp_map);
						delete temp_map;
					}
				}
			}
			//trace("Best combo step(" + best_chain_step_x + "," + best_chain_step_y + ")");
			//trace("shoot chain(" + this.best_chain_step_x + "," + this.best_chain_step_y + ")");
			//map.lookup(best_chain_step_x,best_chain_step_y).i_am_hit(1);
		}
	}
	private function amass_travel(map: Map){
		if(true){
			var temp_point = 0;
			var amass_point = amass_counter(map);
			for(var h = map.Height-1; h > this.travel_limit; --h){
				for(var w = map.Width-1; w >= 0 ; --w){
					if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
						var temp_map: Map = set_brain_map(map);
						del_block(temp_map, w, h, true);
						temp_point = amass_counter(temp_map);
						if(temp_point > amass_point){
							amass_point = temp_point;
						}
						delete temp_map;
					}
				}
			}
			//trace("Best amass step(" + best_amass_step_x + "," + best_amass_step_y + ")");
			if(this.best_amass_step_x != -1){
				//trace("shoot amass(" + this.best_amass_step_x + "," + this.best_amass_step_y + ")");
				//map.lookup(best_amass_step_x,best_amass_step_y).i_am_hit(1);
			}
		}
	}
	
	private function amass_travel_deep(map: Map){
		if(true){
			var amass_point = 0;
			var chain_point = 0;
			amass_point = amass_counter(map);
			for(var h = map.Height-1; h > this.travel_limit; --h){
				for(var w = map.Width-1; w >= 0 ; --w){
					if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
						var temp_map: Map = set_brain_map(map);
						del_block(temp_map, w, h, true);
						amass_point = amass_counter(temp_map);
						for(var i = this.best_chain_cubes.length; i >=0; --i){
							if(w != this.best_chain_cubes[i].x){
								var temp_map_2: Map = set_brain_map(temp_map);
								del_block(temp_map_2,this.best_chain_cubes[i].x,this.best_chain_cubes[i].y,true);
								chain_point = combo_counter(temp_map_2);
								delete temp_map_2;
							}else if(w = this.best_chain_cubes[i].x && h > this.best_chain_cubes[i].y){
								var temp_map_2: Map = set_brain_map(temp_map);
								del_block(temp_map_2,this.best_chain_cubes[i].x,this.best_chain_cubes[i].y+1,true);
								chain_point = combo_counter(temp_map_2);
								delete temp_map_2;
							}
						}
						delete temp_map;
					}
				}
			}
		}
	}
	
	public function dropping_travel(map: Map){
		var del_arr = Dropping_checker(map);
		if(del_arr != null){
			for(var del_i = 0; del_i < del_arr.length; ++del_i){
				del_arr[del_i].i_am_hit(1);
				trace("del_dropping(" + del_arr[del_i].x + "," + del_arr[del_i].y + ")");
			}
		}
	}
	
	//count amass then get point
	private function amass_counter(map: Map): Number{
		if(contact_checker(map, 3) == null){
			var get_point: Number = 0;
			get_point = contact_checker(map, 2).length;
			return get_point;
		}
		return 0;
	}
	
	//count chain combo then get point
	private function combo_counter(map: Map): Number{
		var square_count: Array = new Array();
		var chain_count: Number = 0;
		var get_point: Number = 0;
		while(contact_checker(map,3) != null){
			//use Array to save contace_checker
			var chain_arr: Array = new Array();
			chain_arr = contact_checker(map,3);
			++chain_count;
			square_count.push(chain_arr.length);
			chain_del(map, chain_arr);
		}
		//point_formula : (del_square_in_each_chain-3)*each_chain_Num+2^(each_chain_Num-2)
		for(var i = 0; i < square_count.length; ++i){
			get_point += (square_count[i]-3)*i+1;
			if( i == 0 ){
				get_point += (square_count[i]-3)*(i+1);
			}else{
				get_point += (square_count[i]-3)*(i+1) + Math.pow(2, i-1);
			}
		}
		return get_point;
	}
	//treavl map to find chain or two_contact
	private function contact_checker(map: Map, contact_Num: Number): Array{
		var temp_arr: Array = new Array();
		var push_arr: Array = new Array();
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.make_row(map.lookup(w,h)).length >= contact_Num-1){
					push_arr = map.make_row( map.lookup(w,h) );
					push_arr.push( map.lookup(w,h) );
					for(var push_row = 0; push_row < push_arr.length; ++push_row){
						if(push_arr[push_row].cycled == false){
							push_arr[push_row].cycled = true;
							temp_arr.push(push_arr[push_row]);
						}
					}
				}
				if(map.make_column(map.lookup(w,h)).length >= contact_Num-1){
					push_arr = map.make_column( map.lookup(w,h) );
					push_arr.push( map.lookup(w,h) );
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
		if(temp_arr.length == 0){
			return null;
		}else{
			return temp_arr;
		}
	}
	//return cube arrry if dorpping cubes will make incorrect chain
	//maybe we can use tag of true map to check what kind the dorpping cube is 
	private function Dropping_checker(true_map: Map): Array{
		var temp_map: Map = set_brain_map(true_map);
		var check_arr: Array = new Array();
		var dropping_arr: Array = new Array();
		var del_arr: Array = new Array();
		for(var h = true_map.Height-1; h > 0; --h){
			for(var w = true_map.Width-1; w >= 0 ; --w){
				if(true_map.lookup(w, h).state instanceof Dropping && true_map.lookup(w, h).first_drop == true){
					dropping_arr.push(true_map.lookup(w, h));
					check_arr.push(temp_map.lookup(w,h));
				}
			}
		}
		for(var h = temp_map.Height-1; h > 0; --h){
			for(var w = temp_map.Width-1; w >= 0 ; --w){
				if(temp_map.lookup(w, h) == null){
					move_block(temp_map, w, h, true);
				}
			}
		}
		if(contact_checker(temp_map, 3) == null){
			delete temp_map;
			return null;
		}else{
			for(var check_i = 0; check_i < check_arr.length; ++check_i){
				if(temp_map.make_row(check_arr[check_i]).length >= 2){
					del_arr.push(dropping_arr[check_i]);
				}else if(temp_map.make_column(check_arr[check_i]).length >= 2){
					del_arr.push(dropping_arr[check_i]);
				}
			}
			delete temp_map;
			return del_arr;
		}
	}
	
	private function chain_del(brain_map: Map, del_arr: Array){
		//delete del_arr without sort
		for(var cube_Num = 0; cube_Num < del_arr.length; ++cube_Num){
			del_block(brain_map, del_arr[cube_Num].x, del_arr[cube_Num].y, false);
		}
		//sort map
		for(var h = brain_map.Height-1; h > 0; --h){
			for(var w = brain_map.Width-1; w >= 0 ; --w){
				if(brain_map.lookup(w, h) == null){
					move_block(brain_map, w, h, false);
				}
			}
		}
	}
	
	private function del_block(brain_map: Map, x: Number, y: Number, need_move: Boolean){
		var del_x = x;
		var del_y = y;
		brain_map[del_y][del_x].suicide();
		brain_map.delData(del_x, del_y);
		if(need_move == true){
			move_block(brain_map, del_x, del_y, false);
		}
	}
	
	private function move_block(brain_map: Map, x: Number, y: Number, include_dropping: Boolean){
		var locx = x;
		var locy = y;
		for(var target_y = locy-1;
			target_y > 0 &&
			brain_map.lookup(locx,locy) == null;
			--target_y){
			if(brain_map.lookup(locx,target_y) != null){
				if(include_dropping == false && brain_map.lookup(locx,target_y).state instanceof Waiting){
					brain_map.setup(locx, locy, brain_map.lookup(locx, target_y));
					brain_map.lookup(locx, locy).y = locy;
					//trace("move dorpping cube(" + locx + "," + target_y + ") to (" + locx + "," + locy + ")");
					del_block(brain_map, locx, target_y, true);
				}else if(include_dropping == true && brain_map.lookup(locx,target_y).state instanceof Dropping){
					brain_map.setup(locx, locy, brain_map.lookup(locx, target_y));
					brain_map.lookup(locx, locy).y = locy;
					//trace("move waiting  cube(" + locx + "," + target_y + ") to (" + locx + "," + locy + ")");
					del_block(brain_map, locx, target_y, true);
				}
			}
		}
	}
	
	private function trace_map_state(map: Map){
		var state_arr: Array = new Array(map.Height);
        for(var i = 0; i < map.Height; ++i){
			state_arr[i] = new Array(map.Width);
		}
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if( map.lookup(w,h).state instanceof Waiting ){
					var waiting: Waiting = Waiting(map.lookup(w,h).state);
					if( waiting.state_checked() == true){
						state_arr[h][w] = "WT";
					}else if( waiting.state_checked() == false){
						state_arr[h][w] = "WF";
					}else{
						state_arr[h][w] = "Wa";
					}
				}else if ( map.lookup(w,h).state instanceof Dropping ){
					if(map.lookup(w,h).first_drop == true){
						state_arr[h][w] = "DT";
					}else if(map.lookup(w,h).first_drop == false){
						state_arr[h][w] = "DF";
					}else{
						state_arr[h][w] = "Dr";
					}
				}else if ( map.lookup(w,h).state instanceof Dying )
				    state_arr[h][w] = "Dy";
				else
				    state_arr[h][w] = "--";
			}
		}
		trace("map_state:");
		for(var a = 0; a < map.Height; ++a){
			trace(state_arr[a]);
		}
	}
	
	private function trace_map_rgb(map: Map){
		var rgb_arr: Array = new Array(map.Height);
        for(var i = 0; i < map.Height; ++i){
			rgb_arr[i] = new Array(map.Width);
		}
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if( map.lookup(w,h).rgb == 255 << 16 )
				    rgb_arr[h][w] = "Re";
				else if ( map.lookup(w,h).rgb == 255 << 8 )
				    rgb_arr[h][w] = "Gr";
				else if ( map.lookup(w,h).rgb == 255 )
				    rgb_arr[h][w] = "Bl";
				else if ( map.lookup(w,h).rgb == (255<<16) + (255<<8) )
				    rgb_arr[h][w] = "Ye";
				else
				    rgb_arr[h][w] = "--";
			}
		}
		trace("map_RGB:");
		for(var a = 0; a < map.Height; ++a){
			trace(rgb_arr[a]);
		}
	}
	private function trace_map_cycled(map: Map){
		var cycled_arr: Array = new Array(map.Height);
        for(var i = 0; i < map.Height; ++i){
			cycled_arr[i] = new Array(map.Width);
		}
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w,h).cycled == true ){
					cycled_arr[h][w] = "Tr";
				}else if(map.lookup(w,h).cycled == false ){
					cycled_arr[h][w] = "Fa";
				}else{
					cycled_arr[h][w] = "--";
				}
			}
		}
		trace("map_cycled:");
		for(var a = 0; a < map.Height; ++a){
			trace(cycled_arr[a]);
		}
	}
	
	private var travel_limit: Number;
	private var best_amass_step_x: Array;
	private var best_amass_step_y: Array;
	private var best_amass_point: Array;
	private var best_chain_step_x: Array;
	private var best_chain_step_y: Array;
	private var best_chain_point: Array;
	private var best_chain_cubes: Array;
	private var clear_garbage_x: Array;
	private var clear_garbage_y: Array;
	private var shooted: Boolean;
	private var chaining: Boolean;
	private var game_: Game;
	private var map_: Map;
}