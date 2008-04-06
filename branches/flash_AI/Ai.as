
class Ai{
	public function Ai(game: Game, map: Map){
		game_ = game;
		map_ = map;
		travel_limit = 3;//make AI only travel the last (11-limit) Row of Map
		best_amass_point = [0,0,0];
		best_amass_cube = null;
		best_chain_point = [0,0,0];
		best_chain_cubes = [null,null,null];//儲存最佳的3個發火點
		shooted = false;
		chaining = false;
		trace("Hi,I am Ai!");
	}
	
	public function full_process(map: Map){
		for(var h = map.Height-1; h > this.travel_limit; --h){
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
		//var bg_list: Array = bk_gbg_travel(map);
		/*if(this.shooted != true){
			bk_gbg_travel(map);
			if(count_square_Num(map) >= 30 || top_checker[map.Width-1+2] <= 2){
				this.best_chain_cubes[2].i_am_hit(1);
			}else if(count_square_Num(map) >= 10){
				amass_travel_deep(map);
			}
		}*/
		//trace_map_rgb(map);
		/*var del_arr: Array = too_high_travel(map);
		for(var i = 0; i<del_arr.length; ++i){
			if(del_arr[i]!=null){
				for(var j=0;j<del_arr[i].length; ++j){
					del_arr[i][j].i_am_hit(1);
				}
			}
		}*/
		//var temp_map: Map = set_brain_map(map);
		//var temp_best_chain_cubes: Array = [null,null,null];
		/*for(var k = 0; k < this.best_chain_point.length; ++k){
			temp_best_chain_cubes[k] = temp_map.lookup(best_chain_cubes[k].x,best_chain_cubes[k].y);
			del_block(temp_map, temp_best_chain_cubes[k].x, temp_best_chain_cubes[k].y+1, true);
			trace("temp combo<" + best_chain_point[k] + ">(" + temp_best_chain_cubes[k].x + "," + temp_best_chain_cubes[k].y + ")");
			trace("Best combo<" + best_chain_point[k] + ">(" + best_chain_cubes[k].x + "," + best_chain_cubes[k].y + ")");
		}*/
	}
	//clone true map to think
	private function set_brain_map (map: Map): Map{
		var clone_map: Map = map.clone();
		return clone_map;
	}
	//射擊目標方塊,對最佳發火點分數進行更新
	private function shooting(map:Map, cube: Square){
		cube.i_am_hit(1);
		best_chain_point_update(map);
	}
	
	//更新最佳的3個發火點
	private function combo_rank_update(map: Map){
		var get_point: Number = 0;
		var best_chain_list: Array = new Array(2);
		best_chain_list[0] = new Array(3);
		best_chain_list[1] = new Array(3);
		best_chain_list[0] = [0,0,0];
		best_chain_list[1] = [null,null,null];
		for(var h = map.Height-1; h > this.travel_limit; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
					var temp_map: Map = set_brain_map(map);
					del_block(temp_map, w, h, true);
					get_point = combo_counter(temp_map);
					for(var i = best_chain_list[0].length; i >=0; --i){
						if(get_point > best_chain_list[0][i]){
							for(var j = 0; j < i; ++j){
								best_chain_list[0][j] = best_chain_list[0][j+1];
								best_chain_list[1][j] = best_chain_list[1][j+1];
							}
							best_chain_list[0][i] = get_point;
							best_chain_list[1][i] = map.lookup(w,h);
							break;
						}
					}
					delete temp_map;
				}
			}
		}
		for(var k = 0; k < this.best_chain_cubes.length; ++k){
			trace("Best combo<" + best_chain_list[0][k] + ">(" + best_chain_list[1][k].x + "," + best_chain_list[1][k].y + ")");
		}
		best_chain_point = best_chain_list[0];
		best_chain_cubes = best_chain_list[1];
	}
	
	private function best_chain_point_update(map: Map){
		for(var i = 0; i<this.best_chain_cubes.length; ++i){
			var temp_map: Map = set_brain_map(map);
			del_block(temp_map, this.best_chain_cubes[i].x, this.best_chain_cubes[i].y, true);
			this.best_chain_point[i] = combo_counter(temp_map);
		}
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
		
	private function top_checker(map: Map): Array{
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
	
	private function rtc_checker(map):Square{
		var temp_map = set_brain_map(map);
		var true_failing_cubes: Array = new Array;
		var temp_failing_cubes: Array = new Array;
		var temp_failing_cubes_2: Array = new Array;
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w, h).state instanceof Dropping && map.lookup(w, h).first_drop != true){
					true_failing_cubes.push(map.lookup(w, h));
					temp_failing_cubes.push(temp_map.lookup(w,h));
				}
				if(map.lookup(w, h) == null){
					for(var y_ = h; y_ > 0; --y_){
						if(map.lookup(w, y_).state instanceof Dropping && map.lookup(w, y_).first_drop != true){
							
							break;
						}
					}
				}
			}
		}
		failing_dropping(temp_map,false);
		if(contact_checker(temp_map,3)!=null){
			return null;
		}else{
			for(var h_ = map.Height-1; h_ > 0; --h_){
				for(var w_ = map.Width-1; w_ >= 0 ; --w_){
					if(map.lookup(w_,h_)!= null && map.lookup(w_,h_).state instanceof Waiting){
						var temp_map_2: Map = set_brain_map(temp_map);
						temp_failing_cubes_2 = new Array;
						for(var i = 0; i<temp_failing_cubes.length; ++i){
							temp_failing_cubes_2.push(temp_map_2.lookup(temp_failing_cubes[i].x,temp_failing_cubes[i].y));
						}
						del_block(temp_map_2, w_, h_, true);
						var temp_rtc_list: Array = new Array;
						for(var j = 0; j<temp_failing_cubes_2.length; ++j){
							temp_chain_list = contact_checker_unit(temp_map_2, temp_map_2.lookup(temp_failing_cubes_2[j].x,temp_failing_cubes_2[j].y), temp_rtc_list, 3);
						}
						if(temp_rtc_list.length !=0){
							for(var recy=0; recy<temp_rtc_list.length; ++recy){
								temp_rtc_list[recy].cycled = false;
							}
						}
						delete temp_map_2;
					}
				}
			}
		}
	}
	
	private function too_high_travel(map: Map): Array{
		var map_tops: Array = top_checker(map);//get high of each column
		var del_list: Array = new Array(map.Width);//to save cubes that need delete
		for(var i = 0; i < del_list.length; ++i){
			del_list[i] = null;
		}
		for(var w = map.Width-1; w > 0; --w){
			//如此列高度超過平均值2
			if(map_tops[w] - map_tops[map.Width-1+1] > 2){
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
		for(var h = map.Height-1; h > 0; --h){
			if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
				//trace("check(" + h + ")>>" + map.make_column(map.lookup(w,h)).length);
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
	//暴搜盤面,找出broken或是garbage方塊並回傳
	private function bk_gbg_travel(map: Map): Array{
		var bg_cubes: Array = new Array(2);
		bg_cubes[0] = new Array;//儲存broken_cubes
		bg_cubes[1] = new Array;//儲存garbage_cubes
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0; --w){
				if(map.lookup(w,h).state instanceof Waiting && map.lookup(w,h).if_broken() == true){
					trace("push broken(" + w + "," + h +")");
					bg_cubes[0].push(map.lookup(w,h));
				}else if(map.lookup(w,h).state instanceof Waiting && map.lookup(w,h).if_garbage() == true){
					trace("push garbage(" + w + "," + h +")");
					bg_cubes[1].push(map.lookup(w,h));
				}
			}
		}
		if(bg_cubes[0].length!=0 || bg_cubes[1].length!=0){
			return bg_cubes;
		}else{
			return null;
		}
	}
		
	//暴搜盤面找出最佳發火點並回傳
	private function combo_travel(map: Map): Square{
		var combo_point: Number = 0;
		var best_cube: Square = null;
		for(var h = map.Height-1; h > this.travel_limit; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
					var temp_map: Map = set_brain_map(map);
					del_block(temp_map, w, h, true);
					if(combo_counter(temp_map)>combo_point){
						combo_point = combo_counter(temp_map);
						best_cube = map.lookup(w,h);
					}
					delete temp_map;
				}
			}
		}
		return best_cube;
	}
	//暴搜盤面找出最佳發火點並回傳(使用combo_rank_udate)
	private function combo_travel_ver2(map: Map): Square{
		combo_rank_update(map);
		return this.best_chain_cubes[2];
	}
	
	private function amass_travel(map: Map): Square{
		var get_point: Number = 0;
		var best_cube: Square = null;
		var amass_point = amass_counter(map);
		for(var h = map.Height-1; h > this.travel_limit; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting){
					var temp_map: Map = set_brain_map(map);
					del_block(temp_map, w, h, true);
					get_point = amass_counter(temp_map);
					if(get_point > amass_point){
						amass_point = get_point;
						best_cube = map.lookup(w,h);
					}
					delete temp_map;
				}
			}
		}
		return best_cube;
	}
	
	private function amass_travel_deep(map: Map): Array{
		var best_cube: Square = null;
		var in_best_cubes: Boolean = false;
		var best_point: Number = 0;
		var result_: Array = new Array(2);//return
		result_[0] = null;//最佳排列點方塊
		result_[1] = new Array(3);//元最佳發火點消去最佳排列解後的新得點
		result_[1] = [0,0,0];
		best_point = amass_deep_formula(amass_counter(map),this.best_chain_point);
		for(var h = map.Height-1; h > this.travel_limit; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				for(var i = this.best_chain_cubes.length; i >=0; --i){
					if(w == this.best_chain_cubes[a].x && h == this.best_chain_cubes[a].y){
						in_best_cubes = true;
						break;
					}
				}
				if(map.lookup(w,h)!= null && map.lookup(w,h).state instanceof Waiting && in_best_cubes != true){
					var temp_map: Map = set_brain_map(map);//temp_map為消去被評價點(w,h)之後的map
					var temp_best_chain_cubes: Array = [null, null, null];//暫存最佳發火點,以防消去被評價點後造成最佳發火點位址變動
					for(var a = 0; a < this.best_chain_cubes.length; ++a){
						temp_best_chain_cubes[a]=temp_map.lookup(this.best_chain_cubes[a].x,this.best_chain_cubes[a].y);
					}
					del_block(temp_map, w, h, true);//消去被評價點(w,h)
					var chain_point: Array = [0,0,0];
					for(var j = this.best_chain_cubes.length; j >=0; --j){
						if(temp_best_chain_cubes[j]!=null){
							var temp_map_2: Map = set_brain_map(temp_map);
							del_block(temp_map_2,temp_best_chain_cubes[j].x,temp_best_chain_cubes[j].y,true);
							chain_point[j] = combo_counter(temp_map_2);
							delete temp_map_2;
						}
					}
					if(amass_deep_formula(amass_counter(temp_map),chain_point) > best_point){
						best_point = amass_deep_formula(amass_counter(temp_map),chain_point);
						best_cube = map.lookup(w,h);
					}
					delete temp_map;
				}
				in_best_cubes = false;
			}
		}
		return result_;
	}
	
	private function amass_deep_formula(amass_point: Number,chain_points: Array): Number{
		var amass_deep_point = amass_point;
		for(var i = 0; i < chain_points.length; ++i){
			amass_deep_point += chain_points[i];
		}
		return amass_deep_point;
	}
	
	public function dropping_travel(map: Map){
		var del_arr = Dropping_checker(map);
		if(del_arr != null){
			trace_map_rgb(map);
			for(var del_i = 0; del_i < del_arr.length; ++del_i){
				shooting(map,del_arr[del_i]);
				trace("del_dropping(" + del_arr[del_i].x + "," + del_arr[del_i].y + ")");
			}
		}
	}
	
	//計算此盤面的兩兩相臨方塊個數 並回傳
	private function amass_counter(map: Map): Number{
		if(contact_checker(map, 3) == null){
			var get_point: Number = 0;
			get_point = contact_checker(map, 2).length;
			return get_point;
		}
		return 0;
	}
	
	//計算此盤面的連鎖分數並回傳
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
	//暴搜盤面,找出兩兩相鄰或是可連鎖消去的方塊並回傳,以contact_Num決定有幾個同色相連才要回傳
	private function contact_checker(map: Map, contact_Num: Number): Array{
		var temp_arr: Array = new Array();
		var push_arr: Array = new Array();
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				temp_arr = contact_checker_unit(map, map.lookup(w,h), temp_arr, contact_Num);
				
				/*if(map.make_row(map.lookup(w,h)).length >= contact_Num-1){
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
				}*/
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
	
	private function contact_checker_unit(map: Map, cube: Square, list: Array, contact_Num: Number): Array{
		var temp_list: Array = new Array;
		if(map.make_row(cube).length >= contact_Num-1){
			temp_list = map.make_row( cube );
			temp_list.push( cube );
			for(var push_row = 0; push_row < temp_list.length; ++push_row){
				if(temp_list[push_row].cycled == false){
					temp_list[push_row].cycled = true;
					list.push(temp_list[push_row]);
				}
			}
		}
		if(map.make_column(cube).length >= contact_Num-1){
			temp_list = map.make_column( cube );
			temp_list.push( cube );
			for(var push_col = 0; push_col < temp_list.length; ++push_col){
				if(temp_list[push_col].cycled == false){
					temp_list[push_col].cycled = true;
					list.push(temp_list[push_col]);
				}
			}
		}
		return list;
	}
	
	//檢查初落下方塊是否會造成誤消去,有則return一組cube array
	private function Dropping_checker(map: Map): Array{
		var temp_map: Map = set_brain_map(map);
		var check_arr: Array = new Array();
		var dropping_arr: Array = new Array();
		var del_arr: Array = new Array();
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w, h).state instanceof Dropping && map.lookup(w, h).first_drop == true){
					dropping_arr.push(map.lookup(w, h));
					check_arr.push(temp_map.lookup(w,h));
				}
			}
		}
		failing_dropping(temp_map,true);
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
	//傳入一cube array,先消去這些cube,再將消去後造成的空格填滿
	private function chain_del(map: Map, del_arr: Array){
		//delete del_arr
		for(var cube_Num = 0; cube_Num < del_arr.length; ++cube_Num){
			del_block(map, del_arr[cube_Num].x, del_arr[cube_Num].y, false);
		}
		//sort map
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w, h) == null){
					move_block(map, w, h, false);
				}
			}
		}
	}
	//消去map中位於(x,y)的cube,由need_move傳入之值決定是否要將消去後造成的空格填起
	private function del_block(map: Map, x: Number, y: Number, need_move: Boolean){
		var del_x = x;
		var del_y = y;
		map[del_y][del_x].suicide();
		map.delData(del_x, del_y);
		if(need_move == true){
			move_block(map, del_x, del_y, false);
		}
	}
	//傳入map中之空格座標(x,y),向(x,y)上方進行搜尋,將(x,y)上方之方塊強制下移填滿空格
	private function move_block(map: Map, x: Number, y: Number){
		var locx = x;
		var locy = y;
		for(var target_y = locy-1;
			target_y > 0 &&
			map.lookup(locx,locy) == null;
			--target_y){
			if(map.lookup(locx,target_y) != null){
				if(map.lookup(locx,target_y).state instanceof Waiting){
					map.setup(locx, locy, map.lookup(locx, target_y));
					map.lookup(locx, locy).y = locy;
					//trace("move dorpping cube(" + locx + "," + target_y + ") to (" + locx + "," + locy + ")");
					del_block(map, locx, target_y, true);
				}
			}
		}
	}
	//將落下中方塊強制落下,由first_drop決定目標是否為初落下方塊
	private function failing_dropping(map: Map, first_drop: Boolean){
		for(var h = map.Height-1; h > 0; --h){
			for(var w = map.Width-1; w >= 0 ; --w){
				if(map.lookup(w,h).state instanceof Dropping && map.lookup(w,h).first_drop == true && first_drop == true){
					for(var move_to = h+1; move_to < map.Height && map.lookup(w,move_to) == null; ++move_to){
						map.setup(w, move_to, map.lookup(w, h));
						map.lookup(w, move_to).y = move_to;
						del_block(map, w, move_to, false);
						if(move_to+1 == map.Height || map.lookup(w,move_to+1) != null){
							map.lookup(w, move_to).state(new Waiting(map));
						}
					}
				}else if(map.lookup(w,h).state instanceof Dropping && map.lookup(w,h).first_drop == false && first_drop == false){
					for(var move_to = h+1; move_to < map.Height && map.lookup(w,move_to) == null; ++move_to){
						map.setup(w, move_to, map.lookup(w, h));
						map.lookup(w, move_to).y = move_to;
						del_block(map, w, move_to, false);
						if(move_to+1 == map.Height || map.lookup(w,move_to+1) != null){
							map.lookup(w, move_to).state(new Waiting(map));
						}
					}
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
	private var best_amass_cube: Square;
	private var best_amass_point: Array;
	private var best_chain_point: Array;
	private var best_chain_cubes: Array;
	private var shooted: Boolean;
	private var chaining: Boolean;
	private var game_: Game;
	private var map_: Map;
}