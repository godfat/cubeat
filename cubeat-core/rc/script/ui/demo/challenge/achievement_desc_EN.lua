-- encode in UTF8 without BOM

local achievement_desc = {
  -- Do we really want a Tutorial achievement?
  story_1 = { name = "Story Mode - Tommy", hint = "Complete Tommy's storyline." },
  story_2 = { name = "Story Mode - Aki", hint = "Complete Aki's storyline." },
  story_3 = { name = "Story Mode - Sieg", hint = "Complete Sieg's storyline." },
  story_4 = { name = "Story Mode - Lia", hint = "Complete Lia's storyline." },
  story_5 = { name = "Story Mode - Hiro", hint = "Complete Hiro's storyline." },
  story_6 = { name = "Story Mode - Violet", hint = "Complete Violet's storyline." },
  
  highest_chain_4 = { name = "Chaining Journeymen", hint = "Make a chain of 4 consecutive matches." },
  highest_chain_6 = { name = "Chaining Expert", hint = "Make a chain of 6 consecutive matches." },
  single_color_match_9 = { name = "Big One", hint = "Make a match consists of at least 9 cubes of the same color." },
  three_color_match = { name = "Colorful Match", hint = "Make a match consists of at least 3 different colors." },
  garbage_left_60 = { name = "He's Going Down", hint = "Accumulate a huge number of attack cubes." },
  
  efficiency_over_time = { name = "CuBeat Master", hint = "Create more than 90 attack cubes per minute in a versus game." },
  
  emergency_stages_cleared = { name = "Don't Panic", hint = "Clear all Emergency mode challenges." },
  puzzle_stages_cleared = { name = "Mind Over Speed", hint = "Clear all Puzzle mode challenges." },
  score_attack_mastered = { name = "High Score", hint = "Get more than 15000 points in Score Attack challenge." },
  
  two_mice_pvp = { name = "Play Together", hint = "Start the VS Player mode with two USB mice." },
  shoot_opponent = { name = "Outside of the Box", hint = "Shoot at your opponent's cube for the first time.", secret = true },
  overheat = { name = "Take It Slowly...", hint = "There's no need to rush! Take your time." },
  -- tug_of_war = { name = "Zigzagging", hint = "Stay in the Emergency status for more than 25 seconds in a versus game." },
  long_struggle = { name = "That's a Long Fight", hint = "Finish a game in more than 4 minutes.", secret = true },
  
  win_veryhard_no_haste = { name = "It's So Easy I Don't Even", hint = "Win a game against Very Hard AI or above without\npressing Haste button." },
  win_lightning_fast = { name = "Blitzkrieg", hint = "Win a game against Normal AI or above\nin 45 seconds." },
  win_safety_first = { name = "Safety First", hint = "Win a game against Normal AI or above\nwithout entering Emergency status." },
  win_overkill1 = { name = "Overkill", hint = "Win a game against Normal AI or above with\nat least 12 attack cubes unreleased." },
  win_overkill2 = { name = "Overkill Masterfully", hint = "Win a game against Very Hard AI or above with\nat least 24 attack cubes unreleased.", secret = true },
  win_so_close = { name = "Close Call", hint = "Win a game with less than 1 second to defeat.", secret = true },
  win_turn_the_tide = { name = "Turn the Tide", hint = "Win a game after being very close to defeat." },
  win_underdog = { name = "Underdog Win", hint = "Win a game against Normal AI or above with\na score 2000 less than your opponent.", secret = true },
  lose_careless = { name = "What Just Happened?", hint = "Lose a game against Normal AI or above with\na score 3000 more than your opponent.", secret = true },
}

return achievement_desc
