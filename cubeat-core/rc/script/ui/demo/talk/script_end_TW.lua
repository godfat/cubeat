local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[
[pos]
change text & panel position
example=> pos={x=700, y=300}

[board]
change panel texture
example=> board="area_rect"

[board_flip]
"H"  = flip panel Horizon
"V"  = flip panel Vertical
"HV" = flip panel Horizon+Vertical

[ch]
1 = left actor
2 = right actor

[effect_a]
show
hide
slide_in
slide_out
fade_in
fade_out
shake

[effect_w]
size_L
size_M
size_S
shake

[special]
cube: x1,y1,x2,y2,x3,y3,path,w,h,dur

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/normal", text="原來這樣就可以把方塊消掉了。"},
  { ch=1, img="char1_new/glad", text="好像…不知不覺間得到了很特別的\n力量呢！" },
  { ch=1, img="char1_new/normal", text="得要取個很厲害的名字才行，\n方塊…打擊…" },
  { ch=1, img="char1_new/glad", text="就叫做「CuBeat」吧！"},
  { ch=1, img="char1_new/glad",text="超～有趣的～！" },
  { ch=1, img="char1_new/glad",text="快點去跟大家炫耀一下！" },
  
  { ch=1, text="" }
}
local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="輕輕鬆鬆啦～"},
  { ch=2, img="char2_new/sad", text="嗚～雅季輸了～", board_flip="HV"},
  { ch=1, img="char1_new/normal", text="不過這麼看來，除了我之外，\n還有人也得到了CuBeat能力呢。" },
  { ch=1, img="char1_new/glad", text="很好！\n那我就要成為最強的CuBeat能力者！"}, 
  
  { ch=1, text="" }
}
local act1_act3_ = {
  { ch=1, img="char1_new/glad", text="哈！真是有趣！"}, 
  { ch=2, img="char3_new/sad", text="…難以置信…",board_flip="H" }, 
  { ch=1, img="char1_new/glad", text="那麼該幫我查查還有誰也獲得\nCuBeat能力了吧？"},
  
  { ch=1, text="" }
}
local act1_act4_ = {
  { ch=1, img="char1_new/glad", text="果然妳也有CuBeat能力嘛！\n不過還是我比較厲害！"},
  { ch=2, img="char4_new/sad", text="本小姐才沒有輸，\n是你的吵鬧干擾了本小姐…",board_flip="HV", pos={x=480, y=230}},
  { ch=1, img="char1_new/normal", text="那麼下一個要找誰呢…？" },
  { ch=1, img="char1_new/normal", text="嘿，\n妳知道還有誰會CuBeat嗎？" },
  { ch=2, img="char4_new/anger", text="本小姐沒有\n回答你的必要！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}}, 
  
  { ch=1, text="" }
}
local act1_act5_ = {
  { ch=1, img="char1_new/glad", text="好耶！我贏了！" },
  { ch=2, img="char5_new/anger", text="不錯嘛，棒球小弟。" ,board_flip="HV"},
  { ch=2, img="char5_new/normal", text="不過，可不要以為勝過了在下，\n你就是最強了。" },
  { ch=1, img="char1_new/anger", text="學長的意思是？" },
  { ch=2, img="char5_new/anger", text="成為最強之人的路途上，\n最大的敵人不會是他人，" },
  { ch=2, img="char5_new/glad", text="而是你自己！" },  
  { ch=1, img="char1_new/anger", text="我自己！？" },
  { ch=2, img="char5_new/glad", text="沒錯！年輕人！\n繼續戰鬥下去吧！" },  
  { ch=2, img="char5_new/glad", text="挑戰一切阻礙！\n挑戰所有對手！" },  
  { ch=2, img="char5_new/glad", text="挑戰自我的極限！\n向著夕陽奔跑吧！" },   
  { ch=2, img="char5_new/glad", text="你的戰鬥將永無止境！" },   
  { ch=1, img="char1_new/glad", text="喔喔喔喔喔喔！！", board="dialogue_bubble_2", effect_w="shake" },
  { ch=2, img="char2_new/glad", text="哇哇！嵐泰好熱血喔！" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="這什麼少年漫畫式的標準結局。" ,effect_a="slide_in"},  
  { ch=2, img="char6_new/anger", text="別太囂張了。" ,effect_a="slide_in"},  
  { ch=2, img="char3_new/glad", text="反正故事總該有個結局嘛。" ,board_flip="V",effect_a="slide_in"},  
  { ch=1, img="char1_new/glad", text="向著夕陽奔跑！", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}
local act1_act6_ = {
  { ch=1, img="char1_new/glad", text="哈哈！贏了贏了！"},
  { ch=2, img="char6_new/sad", text="……。",board_flip="H", pos={x=300, y=140}},
  { ch=1, img="char1_new/normal", text="…還真的變成方塊了。" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/glad", text="雅季贏了呢～",board_flip="V"},
  { ch=2, img="char1_new/sad", text="怎麼會這樣…" ,board_flip="HV"},
  { ch=1, img="char2_new/normal", text="不過這個方塊遊戲是怎麼回事呢？", board_flip="V"},
  { ch=2, img="char1_new/normal", text="這個叫CuBeat，很有趣吧。"},
  { ch=1, img="char2_new/normal", text="喔～連名字都有啊。\n雅季還是第一次聽說呢。" },
  { ch=2, img="char1_new/glad", text="因為是我取的名字！\n剛剛才取的！"},
  { ch=2, img="char1_new/anger", text="至於這些方塊到底是怎麼回事！\n我一點都不知道！"},
  { ch=2, img="char1_new/anger", text="只要很有趣就夠了！" ,board="dialogue_bubble_2", effect_w="shake",effect_a="shake" },
  { ch=2, img="char1_new/sad", text="只是沒想到找妳當第一個對手就輸\n了…",board="dialogue_bubble_1", effect_w="size_M" },
  { ch=2, img="char1_new/sad", text="……。"},
  { ch=1, img="char2_new/normal", text="啊，進入沮喪狀態了。" },
  { ch=1, img="char2_new/normal", text="學校裡還有沒有人知道這些方塊的\n事呢？" },
  
  { ch=1, text="" }
}
local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="啊，已經是放學時間了呢。"},
  
  { ch=1, text="" }
}
local act2_act3_ = {
  { ch=2, img="char3_new/sad", text="這一定有什麼誤會…", board="dialogue_bubble_1",board_flip="H"}, 
  { ch=1, img="char2_new/glad", text="耶～贏了～",board_flip="V"},
  { ch=1, img="char2_new/glad", text="雅季除了有時間控制能力之外，" },
  { ch=1, img="char2_new/glad", text="說不定還有「玩CuBeat很厲害」的\n能力呢！" },
  
  { ch=1, text="" }
}
local act2_act4_ = {
  { ch=1, img="char2_new/glad", text="連俐亞都贏了！耶～耶～" ,board_flip="V"},
  { ch=2, img="char4_new/anger", text="才沒那回事！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  { ch=2, img="char4_new/sad", text="我、我是…\n因為妳是本小姐的朋友，\n才放水讓妳贏的！",board_flip="HV",effect_w="size_M" , board="dialogue_bubble_1"},
  { ch=2, img="char4_new/sad", text="本小姐要繼續畫圖了，\n不要打擾我！"},
  { ch=1, img="char2_new/normal", text="耶～\n這樣雅季只好再去找別人玩了。"  ,board_flip="V"},
  
  { ch=1, text="" }
}
local act2_act5_ = {
  { ch=1, img="char2_new/glad", text="雅季很厲害吧！\n這樣我還需要鍛鍊嗎？",board_flip="V"},
  { ch=2, img="char5_new/sad", text="…看來是在下需要上山\n找叢林野人鍛鍊了…" ,board_flip="H"},
  
  { ch=1, text="" }
}
local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="耶～大獲全勝！" ,board_flip="V"},
  { ch=2, img="char6_new/sad", text="……。" ,board_flip="H"},
  { ch=1, img="char2_new/normal", text="可是問了這麼多人，\n還是沒有人知道CuBeat到底是怎麼\n回事呢。" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="雅季還以為雪青是最有可能知道答\n案的說。" },
  { ch=1, img="char2_new/sad", text="會不會只是我睡昏頭了呢？",board_flip="V" },
  { ch=1, img="char2_new/normal", text="啊，對了！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="這一定是那個，\n電視劇裡最老套的發展吧。" },
  { ch=1, img="char2_new/glad", text="我一定是在作夢呢！" ,board_flip="V"},
  { ch=2, img="char1_new/glad", text="那還真是個有趣的夢！" ,board_flip="V" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="這樣就想交代過去也太隨便了吧。"  ,effect_a="slide_in"}, 
  { ch=2, img="char6_new/sad", text="哼…。",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char5_new/glad", text="就算在夢中一樣要鍛鍊！",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char3_new/anger", text="喂喂，夢結局\n是被禁止的啊！" , board="dialogue_bubble_2", effect_w="shake",effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="不要叫醒我喔！" },
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=2, img="char1_new/sad", text="怎麼會…",board_flip="HV" },
  { ch=1, img="char3_new/glad", text="下次跟人宣戰前，\n記得先看清楚對手是誰。"},
  { ch=1, img="char3_new/glad", text="不過就算你沒跟我宣戰，\n我也會這麼做的，科科。" },
  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=2, img="char2_new/sad", text="你們都欺負雅季啦～",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="剛剛有說到嵐泰對吧，\n下一個目標鎖定！"},
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="很好，再來就是調查一下有哪些人\n具有方塊控制能力…"},
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=1, img="char3_new/glad", text="跟我推測的一樣，\n妳果然有控制方塊的能力。"},
  { ch=1, img="char3_new/glad", text="只是技巧還有待加強就是，\n科科。"},
  { ch=2, img="char4_new/sad", text="…你這種讚美方式本小姐聽不懂…",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="下一個。"},
  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/glad", text="現在是誰的頭腦比較需要鍛鍊啊？"},
  { ch=2, img="char5_new/sad", text="唔……" ,board_flip="H"},
  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/glad", text="二話不說就開打也太衝動了吧，\n科科。"},
  { ch=2, img="char6_new/sad", text="……。",board_flip="H"},
  { ch=1, img="char3_new/normal", text="不過，\n妳跟其他人的狀況不同呢。"},
  { ch=1, img="char3_new/normal", text="明明身上沒有方塊形配件，\n卻擁有CuBeat能力。"},
  { ch=1, img="char3_new/normal", text="這之中一定有什麼值得研究的關鍵。"},
  { ch=1, img="char3_new/glad", text="還是說，妳把它藏在哪個\n神.祕.的.地.方.啦？\n科科。"},
  { ch=2, img="char1_new/glad", text="緊張緊張刺激刺激，\n隱藏在CuBeat背後的天大秘密即將\n揭曉！",board_flip="V",effect_a="slide_in"},
  { ch=2, img="char2_new/glad", text="欲知詳情，請待下回分曉！",effect_a="slide_in"},
  { ch=1, img="char3_new/anger", text="你們兩個突然跑出來做什麼旁白啊！"},
  { ch=2, img="char5_new/glad", text="鍛鍊永無止盡，\n這故事也不會到此為止！",effect_a="slide_in"},
  { ch=2, img="char4_new/glad", text="不知道你們在說什麼，\n不過都結尾了本小姐當然要出來露\n個臉囉。",effect_a="slide_in"},  
  { ch=2, img="char6_new/sad", text="……。",board_flip="V",effect_a="slide_in"},
  { ch=1, img="char3_new/anger", text="等等，為什麼我的破關結局是這種\n「未完待續」！？", effect_a="shake", board="dialogue_bubble_2"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/glad", text="想要來挑戰本小姐，\n程度不該只有這樣而已吧。" },
  { ch=2, img="char1_new/sad", text="妳果然是CuBeat能力者…\n而且好強…" ,board_flip="HV"},
  { ch=2, img="char1_new/sad", text="我還以為只有阿廣學長可以跟我相\n抗衡…" },
  { ch=1, img="char4_new/normal", text="阿廣學長？" },
  
  { ch=1, text="" }
}
local act4_act2_ = {
  { ch=1, img="char4_new/glad", text="怎麼樣，本小姐的技巧？" },
  { ch=2, img="char2_new/sad", text="美喵～" ,board_flip="HV"},
  { ch=1, img="char4_new/normal", text="隨便妳了。" },
  
  { ch=1, text="" }
}
local act4_act3_ = {
  { ch=1, img="char4_new/glad", text="怎麼樣，\n這下該領悟到本小姐的厲害了吧？"},
  { ch=2, img="char3_new/sad", text="竟然有這種事…",board_flip="H"}, 
  { ch=1, img="char4_new/glad", text="盡情把本小姐的名號散播出去吧！"},
  
  { ch=1, text="" }
}
local act4_act4_ = {
  { ch=1, img="char4_new/glad", text="喔呵呵，\n本小姐的畫筆可能比我自己想像的\n還厲害呢。"},
  { ch=1, img="char4_new/glad", text="就讓本小姐來把這個世界變得更加\n美麗吧！" },
  
  { ch=1, text="" }
}
local act4_act5_ = {
  { ch=2, img="char5_new/sad", text="忘、忘了熱身…" ,board_flip="H", effect_a="shake"},
  { ch=1, img="char4_new/glad", text="別練柔道了，\n改來練畫畫怎麼樣？" },
  { ch=1, img="char4_new/glad", text="不過本小姐可不收沒天分的社員喔。" },
  { ch=2, img="char1_new/glad", text="我我～我要報名！" ,board_flip="V",effect_a="slide_in"},
  { ch=2, img="char2_new/glad", text="雅季也要加入！" ,effect_a="slide_in"},
  { ch=2, img="char6_new/normal", text="…現在是？" ,effect_a="slide_in"},
  { ch=2, img="char3_new/normal", text="這什麼莫名其妙的展開？" ,effect_a="slide_in"},
  { ch=2, img="char5_new/glad", text="等一等！各位！\n怎麼會是加入美術社呢？" ,effect_a="slide_in"},
  { ch=2, img="char5_new/anger", text="現在更重要的是，\n吾輩還需要探究CuBeat的秘密！" },
  { ch=1, img="char4_new/normal", text="都說了本小姐不知道CuBeat是什麼。" },
  { ch=2, img="char5_new/anger", text="那妳的方塊操作能力該怎麼解釋？" },
  { ch=1, img="char4_new/normal", text="才沒有什麼別的能力呢。" },
  { ch=1, img="char4_new/glad", text="本小姐的畫筆可以改變醜陋的世界，\n這不是理所當然的事嗎？" },
  
  { ch=1, text="" }
}
local act4_act6_ = {
  { ch=1, img="char4_new/glad", text="哎呀，妳真的很幸運呢！" },
  { ch=1, img="char4_new/glad", text="本小姐一不小心就發揮出預期以上\n的水準呢。"},
  { ch=2, img="char6_new/sad", text="……。",board_flip="H"},
  { ch=1, img="char4_new/normal", text="跟妳對話很辛苦耶。"},
  { ch=1, img="char4_new/glad", text="還是妳對本小姐已經敬佩得說不出\n話啦？"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2 , img="char1_new/sad" ,text="不愧是傳說中的阿廣學長…。" ,board_flip="HV"},
  { ch=1 , img="char5_new/glad" ,text="勝敗閑常，\n重點是能否從過程中鍛鍊自我！",board_flip="V" },
  
  { ch=1, text="" }
}
local act5_act2_ = {
  { ch=1 , img="char5_new/glad" ,text="如何！",board_flip="V" },
  { ch=2 , img="char2_new/normal" ,text="如何呢～很有趣啊！" ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="妳即使輸了還是這麼開心？" },
  { ch=2 , img="char2_new/glad" ,text="雅季覺得，\n過程好玩才是最重要的呢～" },
  { ch=1 , img="char5_new/glad" ,text="確實曾經有個武術家說過：\n「答案就在對戰的過程中！」" },
  { ch=2 , img="char2_new/normal" ,text="也許這遊戲就真的只是遊戲，\n除了好玩之外沒有別的目的啊。" },
  { ch=2 , img="char1_new/glad" ,text="沒錯！CuBeat超有趣！",effect_a="slide_in" },
  { ch=2 , img="char4_new/glad" ,text="下次找本小姐切磋前要先預約啊。" ,effect_a="slide_in"},
  { ch=2 , img="char3_new/glad" ,text="差不多就是這樣啦，科科。",board_flip="V" ,effect_a="slide_in"},
  { ch=2 , img="char6_new/normal" ,text="…那個…我…。" ,board_flip="V",effect_a="slide_in"},
  { ch=1 , img="char5_new/glad" ,text="那在下就繼續這挑戰，\n繼續尋求在下的答案吧！" },
  
  { ch=1, text="" }
}
local act5_act3_ = {
  { ch=1 , img="char5_new/glad" ,text="喝！是在下的勝利！" ,board_flip="V"},
  { ch=2 , img="char3_new/sad" ,text="……。" ,board_flip="H"},
  { ch=1 , img="char5_new/normal" ,text="剛剛你話明明還挺多的。" },
  
  { ch=1, text="" }
}
local act5_act4_ = {
  { ch=1 , img="char5_new/glad" ,text="如何！" ,board_flip="V"},
  { ch=2 , img="char4_new/sad" ,text="…本小姐以後絕對不會叫你們這家\n的外賣…" ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="這邊看來是打探不到什麼了，\n接下來…" },
  
  { ch=1, text="" }
}
local act5_act5_ = {
  { ch=1, img="char5_new/glad" ,text="哼！就跟劈磚破瓦沒什麼兩樣！",board_flip="V" },
  { ch=1, img="char5_new/normal" ,text="不過這些方塊是怎麼回事？\n得要搞清楚才行…" },

  { ch=1, text="" }
}
local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="贏是贏了，\n卻連為什麼開打都不清楚，\n總覺得不能接受。",board_flip="V"},
  { ch=2, img="char6_new/sad" ,text="……。",board_flip="H"},
  { ch=1, img="char5_new/normal" ,text="魔術小姐這樣子也問不到話了，\n不過…" },
  { ch=1, img="char5_new/normal" ,text="一提到CuBeat就急著開打，\n彷彿想掩蓋或阻止什麼一樣。" },
  { ch=1, img="char5_new/glad" ,text="事情不單純啊。" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=2, img="char1_new/sad" , text="好強…",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="…如何？\n(輸到不敢再想方塊了吧！)",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="別再提起…CuBeat！"},
  { ch=2, img="char1_new/anger" , text="咦？為什麼！\n這遊戲很有趣的啊！"},
  { ch=1, img="char6_new/normal" , text="…！？\n(什麼！？)"},
  { ch=2, img="char2_new/glad" , text="雅季也這麼覺得！好有趣的說！",effect_a="slide_in" },
  { ch=2, img="char4_new/sad" , text="哼，本小姐才、才不這麼覺得。",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="只不過，\n這碰巧可以讓本小姐展示才能，"},
  { ch=2, img="char4_new/glad" , text="所以願意偶而玩玩罷了。"},
  { ch=2, img="char5_new/glad" , text="在下亦是從來不會拒絕任何挑戰！",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="哼哼，我查出來了，\n原來妳是想抹除掉自己的意外創造\n啊。",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="妳以為現在還來得及嗎，科科。"},
  { ch=2, img="char1_new/glad" , text="今後CuBeat將成為熱門遊戲！",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="…你們！\n(這些傢伙！)"},
  { ch=1, img="char6_new/glad" , text="…！\n(那就這樣落幕吧！)"},

  { ch=1, text="" }
}
local act6_act2_ = {
  { ch=2, img="char2_new/sad" , text="嗚嗚，輸了。",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="嘿嘿！\n(又處理掉一個！)",board_flip="V"},
  
  { ch=1, text="" }
}
local act6_act3_ = {
  { ch=1, img="char6_new/glad" , text="嘿！\n(別小看我！)",board_flip="V"},
  { ch=2, img="char3_new/sad" , text="怎麼可能…",board_flip="H"},
  { ch=1, img="char6_new/normal" , text="嗯…。\n(接著是…)"},
  
  { ch=1, text="" }
}
local act6_act4_ = {
  { ch=1, img="char6_new/glad" , text="嘿！\n(贏了！)",board_flip="V"},
  { ch=2, img="char4_new/sad" , text="明明就是本小姐的畫面比較好看…",board_flip="HV"},
  { ch=1, img="char6_new/normal" , text="喔。\n(看來這傢伙還搞不清楚狀況)"},
  
  { ch=1, text="" }
}
local act6_act5_ = {
  { ch=1, img="char6_new/glad" , text="哈！\n(怎麼樣！)",board_flip="V"},
  { ch=2, img="char5_new/sad" , text="難道在下的努力還不夠！？",board_flip="H"},

  { ch=1, text="" }
}
local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(是我的超能力失控，\n才製造出這些方塊嗎？)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(好像有不少方塊已經散布到校園各\n處了…)"},
  { ch=1, img="char6_new/anger" , text="(這樣的話，我只好…)"},
  
  { ch=1, text="" }
}

local rundown_ = {
  r1_1 = act1_act1_,
  r1_2 = act1_act2_,
  r1_3 = act1_act3_,
  r1_4 = act1_act4_,
  r1_5 = act1_act5_,
  r1_6 = act1_act6_,
  
  r2_1 = act2_act1_,
  r2_2 = act2_act2_,
  r2_3 = act2_act3_,
  r2_4 = act2_act4_,
  r2_5 = act2_act5_,
  r2_6 = act2_act6_,
  
  r3_1 = act3_act1_,
  r3_2 = act3_act2_,
  r3_3 = act3_act3_,
  r3_4 = act3_act4_,
  r3_5 = act3_act5_,
  r3_6 = act3_act6_,
  
  r4_1 = act4_act1_,
  r4_2 = act4_act2_,
  r4_3 = act4_act3_,
  r4_4 = act4_act4_,
  r4_5 = act4_act5_,
  r4_6 = act4_act6_,
  
  r5_1 = act5_act1_,
  r5_2 = act5_act2_,
  r5_3 = act5_act3_,
  r5_4 = act5_act4_,
  r5_5 = act5_act5_,
  r5_6 = act5_act6_,
  
  r6_1 = act6_act1_,
  r6_2 = act6_act2_,
  r6_3 = act6_act3_,
  r6_4 = act6_act4_,
  r6_5 = act6_act5_,
  r6_6 = act6_act6_,
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown_[k]
end


return {
  get_rundown = get_rundown
}