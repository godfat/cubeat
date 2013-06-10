local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[
, pos={x=384, y=70}
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

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/glad", text="下課啦下課啦！" ,effect_a="slide_in" , pos={x=400, y=80}},
  { ch=1, text="今天真是好天氣！\n熱血滿滿的來去練球啦！" },
  { ch=1, img="char1_new/normal", text="咦，等等，那是什麼？", pos={x=300, y=15} },
  { ch=1, img="char1_new/sad", text="痛痛痛..." ,effect_w="shake", board="dialogue_bubble_2", pos={x=384, y=120}, special={id="cube", x1=500, y1=-50, x2=350, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char1_new/sad", text="好像被什麼東西打到了...", effect_w="size_M", board="dialogue_bubble_1" , pos={x=400, y=100}},
  { ch=1, img="char1_new/angry", text="喂！這些方塊是怎麼回事！？\n看我把你們全部打回天上去！" , pos={x=360, y=30}},
  { ch=1, text="..." },
  { ch=1, img="char1_new/normal", text="原來這樣就可以把方塊消掉了", pos={x=300, y=15} },
  { ch=1, img="char1_new/normal", text="好像不知不覺間\n得到了很特別的力量呢！" },
  { ch=1, img="char1_new/normal", text="得要取個很厲害的名字才行，\n方塊...打擊..." },
  { ch=1, img="char1_new/glad", text="就叫做「CuBeat」吧！", pos={x=400, y=80} },
  { ch=1, text="超─有趣的─！" },
  { ch=1, text="快點去跟大家炫耀一下！" },
  { ch=1, text="" }
}

local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="嘿！地味女！還在教室嗎？\n妳已經從上課發呆到放學囉！" , pos={x=400, y=80}},
  { ch=2, img="char2_new/normal", text="..." , board="dialogue_bubble_1" ,board_flip="HV", pos={x=460, y=256}},
  { ch=1, img="char1_new/glad", text="哈，果然還在教室啊！\n呆夠了吧？跟妳說跟妳說..." },
  { ch=2, img="char2_new/normal", text="......" },
  { ch=1, img="char1_new/normal", text="咦？沒聽到嗎？\n喂喂！地味女！" , pos={x=300, y=15}},
  { ch=2, img="char2_new/angry", text="正太男你很吵說！\n地味女在想事情呢。", pos={x=480, y=300} },
  { ch=1, img="char1_new/normal", text="妳看起來明明是在發呆吧。" },
  { ch=2, img="char2_new/sad", text="我覺得頭昏昏的...\n好像被什麼打到頭的感覺。", pos={x=460, y=256} },
  { ch=2, img="char2_new/sad", text="腦袋裡都是方塊的畫面..." },
  { ch=2, img="char2_new/angry", text="而且只要地味女用力想，\n那些方塊就會隨我想的消失..." , pos={x=480, y=300}},
  { ch=1, img="char1_new/angry", text="喂喂...難道說\n妳也得到了CuBeat能力？" , pos={x=360, y=30}},
  { ch=2, img="char2_new/normal", text="QB？那是什麼？" , pos={x=460, y=256}},
  { ch=1, img="char1_new/angry", text="一決勝負吧！" , board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=2, img="char2_new/normal", text="耶～？" },
  { ch=1, img="char1_new/angry", text="看看誰才是最會CuBeat的人！", board="dialogue_bubble_1", effect_w="size_M" },
  { ch=1, img="char1_new/glad", text="輕輕鬆鬆啦" , pos={x=384, y=70}},
  { ch=2, img="char2_new/sad", text="嗚～地味女輸了" },
  { ch=1, img="char1_new/normal", text="不過這麼說的話，\n就代表了除了我之外...", pos={x=280, y=15} },
  { ch=1, img="char1_new/normal", text="還有其他人\n也有了CuBeat能力呢。" },
  { ch=1, img="char1_new/glad", text="好！那我就要\n成為最強的CuBeat能力者！", pos={x=384, y=70} },	
  { ch=1, text="" }
}

local act1_act3_ = {
  { ch=1, img="char1_new/normal", text="喔～電腦社的設備\n看起來很厲害呢。" , pos={x=280, y=15}},
  { ch=1, img="char1_new/normal", text="擅長收集資訊的他們\n一定知道些CuBeat的事情吧？" },
  { ch=2, img="char3_new/normal", text="要幹嘛？" ,board_flip="HV" ,effect_a="slide_in", pos={x=490, y=300}},
  { ch=1, img="char1_new/normal", text="啊，沒什麼啦，\n只是想問看看..." },
  { ch=1, img="char1_new/normal", text="你知道嗎？\n學校出現很多不明方塊呢。" },
  { ch=2, img="char3_new/normal", text="不明方塊？\n你都知道他們是方塊了，\n又哪有什麼不明？"},
  { ch=1, img="char1_new/sad", text="不要抓人語病嘛。" , pos={x=384, y=70}},
  { ch=1, img="char1_new/normal", text="那你知道有些人擁有\n可以把方塊給消除的能力嗎？" , pos={x=280, y=15}},
  { ch=1, img="char1_new/glad", text="這種能力叫做「CuBeat」，\n是我取的名字！" , pos={x=384, y=70}},
  { ch=2, img="char3_new/glad", text="「CuBeat」是嗎？\n你想知道還有哪些人\n也擁有所謂的CuBeat能力？",board_flip="V" },
  { ch=2, img="char3_new/glad", text="哼哼...科科..."},
  { ch=2, img="char3_new/angry", text="我也有啊！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=420, y=230}},
  { ch=1, text="..." },
  { ch=1, img="char1_new/glad", text="哈！真是有趣！" , pos={x=384, y=70}}, 
  { ch=2, img="char3_new/sad", text="...難以置信...", effect_w="size_M" , board="dialogue_bubble_1", pos={x=400, y=260}}, 
  { ch=1, img="char1_new/glad", text="那麼該幫我查查還有誰\n也獲得CuBeat能力了吧？"},
  { ch=1, text="" }
}

local act1_act4_ = {
  { ch=1, img="char1_new/normal", text="美術室...真是壯觀啊。" , pos={x=300, y=15}},
  { ch=1, img="char1_new/normal", text="駭客男給的情報是\n這裡可能有人會CuBeat，\n是真的嗎？" },
  { ch=2, img="char4_new/normal", text="喂，你幹嘛？\n不要打擾本小姐作畫。",board_flip="HV" ,effect_a="slide_in", pos={x=460, y=240}},
  { ch=1, img="char1_new/normal", text="作畫？\n這些圖都是妳畫的嗎？" }, 
  { ch=1, img="char1_new/angry", text="好漂亮！\n真是太厲害了！" , pos={x=360, y=30}},
  { ch=2, img="char4_new/glad", text="呵，可不要以為\n這種程度的誇獎\n就能夠討本小姐歡心啊。" }, 
  { ch=1, img="char1_new/glad", text="那妳一定也有CuBeat能力吧！" , pos={x=400, y=80}},
  { ch=1, img="char1_new/glad", text="就是能夠打擊方塊，\n讓方塊排列消除的能力！"},
  { ch=2, img="char4_new/normal", text="本小姐聽不懂你在說什麼。"},
  { ch=1, img="char1_new/sad", text="耶？\n難道駭客男給的是假情報？" , pos={x=384, y=70}},
  { ch=1, img="char1_new/sad", text="真讓人失望...。"},
  { ch=2, img="char4_new/angry", text="無禮！沒人可以\n對本小姐失望！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=1, img="char1_new/glad", text="果然妳也有CuBeat能力嘛！\n不過還是我比較厲害！" , pos={x=400, y=80}},
  { ch=2, img="char4_new/sad", text="本小姐才沒有輸，\n是你的吵鬧干擾了本小姐...",effect_w="size_M" , board="dialogue_bubble_1", pos={x=520, y=240}},
  { ch=1, img="char1_new/normal", text="那麼下一個要找誰呢...？\n嘿，\n妳知道還有誰會CuBeat嗎？" , pos={x=300, y=15}},
  { ch=2, img="char4_new/angry", text="本小姐沒有\n回答你的必要！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=460, y=240}},
  { ch=1, text="" }
}

local act1_act5_ = {
  { ch=1, img="char1_new/normal", text="翔鯨武館...\n不知不覺就走到這裡了。" , pos={x=280, y=15}},
  { ch=1, img="char1_new/normal", text="那位傳說中的阿廣學長\n不知道在不在？" },
  { ch=1, img="char1_new/glad", text="如果他真有傳說中那麼厲害，\n那只要贏過他我就是最強了！" , pos={x=384, y=70}},
  { ch=2, img="char5_new/normal", text="哈！棒球小弟！\n你也是來鍛鍊身心的嗎？" ,board_flip="HV" ,effect_a="slide_in", pos={x=420, y=250} },
  { ch=1, img="char1_new/normal", text="鍛鍊身心？", pos={x=280, y=15}},
  { ch=1, img="char1_new/normal", text="不是啦，\n我是在找跟我一樣\n有方塊消除能力的人。" },
  { ch=2, img="char5_new/glad", text="喝！\n那棒球小弟你沒有找錯地方！" },
  { ch=1, img="char1_new/angry", text="果然如此！" , pos={x=360, y=30}},
  { ch=1, img="char1_new/angry", text="學長！\n跟我一決勝負吧！\n我要成為最強CuBeat能力者！" },
  { ch=1, img="char1_new/angry", text="如果能贏過你\n我就能成為最強！" },
  { ch=1, img="char1_new/normal", text="...我有這種感覺。", pos={x=280, y=15} },
  { ch=2, img="char5_new/normal", text="想成為最強是嗎？" },
  { ch=2, img="char5_new/angry", text="棒球小弟，\n你可曾想過\n什麼樣才叫做最強！？" },
  { ch=2, img="char5_new/normal", text="成為了最強之後，\n下一步又是什麼？" },
  { ch=1, img="char1_new/angry", text="當然是...", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=360, y=30} },
  { ch=1, img="char1_new/angry", text="是..." , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char1_new/normal", text="對耶，\n這問題我好像沒想過。", pos={x=280, y=15}},
  { ch=2, img="char5_new/angry", text="給我用用頭腦啊！\n年輕人！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake"  },
  { ch=1, img="char1_new/sad", text="你先打贏我再跟我說教吧...。" , pos={x=384, y=70}},
  { ch=2, img="char5_new/glad", text="那在下可不會手下留情！" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, text="" }
}

local act1_act6_ = {
  { ch=1, img="char1_new/normal", text="CuBeat這種不可思議的事情，\n說起來還真像是魔術一樣。" , pos={x=280, y=15}},
  { ch=1, img="char1_new/normal", text="魔術社會不會有什麼情報呢？"},
  { ch=1, img="char1_new/glad", text="哈囉，魔術社的各位大家好！" , pos={x=384, y=70}},
  { ch=2, img="char6_new/normal", text="......" ,board_flip="HV" ,effect_a="slide_in", pos={x=470, y=250} },
  { ch=1, img="char1_new/normal", text="那個...妳好？" , pos={x=280, y=15}},
  { ch=2, img="char6_new/glad", text="......", pos={x=440, y=250}},
  { ch=1, img="char1_new/glad", text="喔喔！好厲害！\n真不愧是魔術師，\n好有趣的打招呼方式！" , pos={x=384, y=70}},
  { ch=2, img="char6_new/normal", text="......嗯。", pos={x=470, y=250}},
  { ch=1, img="char1_new/normal", text="妳果然很會變魔術呢。" , pos={x=280, y=15}},
  { ch=1, img="char1_new/normal", text="那麼有種\n關於方塊的「魔術」，\n妳應該也沒問題吧？"},
  { ch=2, img="char6_new/angry", text="哼！！", pos={x=400, y=300}},
  { ch=1, img="char1_new/angry", text="耶？突然就宣戰！？" , pos={x=360, y=30}},
  { ch=1, img="char1_new/glad", text="哈哈！贏了贏了！" , pos={x=384, y=70}},
  { ch=2, img="char6_new/sad", text="......",board_flip="V"},
  { ch=1, img="char1_new/normal", text="...還真的變成方塊了。" , pos={x=280, y=15}},
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/normal", text="啊，是正太男。", pos={x=320, y=15} },
  { ch=2, img="char1_new/glad", text="地味女！妳來得剛好！\n給妳看件很有趣的事情！"  ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="哇～真的嗎？\n是什麼呢？\n長得像兔子的小貓嗎？" , pos={x=320, y=5} },
  { ch=2, img="char1_new/normal", text="絕對不是。\n那個有趣嗎？" , pos={x=430, y=250}},
  { ch=1, img="char2_new/normal", text="真可惜呢。\n有的話一定很有趣。" },
  { ch=2, img="char1_new/angry", text="不要管什麼阿貓阿狗了！\n來試試這個吧！" , pos={x=410, y=260}},
  
  { ch=1, img="char2_new/glad", text="贏了呢。" },
  { ch=2, img="char1_new/sad", text="怎麼會這樣..." , pos={x=410, y=320}},
  { ch=1, img="char2_new/normal", text="不過這個方塊遊戲\n到底是什麼回事呢。" },
  { ch=2, img="char1_new/normal", text="這個叫CuBeat，很有趣吧。" , pos={x=430, y=250} },
  { ch=1, img="char2_new/normal", text="喔～連名字都有啊。\n地味女還是第一次聽說呢。" },
  { ch=2, img="char1_new/glad", text="因為是我取的名字！\n剛剛才取的！", pos={x=384, y=300} },
  { ch=2, img="char1_new/angry", text="至於這些方塊\n到底是怎麼回事！\n我一點都不知道！", pos={x=410, y=260}},
  { ch=2, img="char1_new/angry", text="只要很有趣就夠了！" },
  { ch=2, img="char1_new/sad", text="只是沒想到找妳\n當第一個對手就輸了..." , pos={x=410, y=320}},
  { ch=2, img="char1_new/sad", text="......"},
  { ch=1, img="char2_new/normal", text="啊，進入沮喪狀態了。" },
  { ch=1, img="char2_new/normal", text="去看看學校裡\n有沒有別人知道\n這是怎麼一回事吧。" },
  { ch=1, text="" }
}

local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="今天的天氣很不錯呢。" , pos={x=320, y=15}},
  { ch=1, img="char2_new/normal", text="有種可以盯著窗外\n看天空一整天的感覺。" },
  { ch=1, img="char2_new/normal", text="這樣很幸福的說。" },
  { ch=1, img="char2_new/sad", text="好痛！" ,effect_w="shake", board="dialogue_bubble_2", pos={x=384, y=120}, special={id="cube", x1=500, y1=-50, x2=300, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char2_new/angry", text="討厭！誰亂打人家的頭！", effect_w="size_M", board="dialogue_bubble_1"  , pos={x=320, y=70} },
  { ch=1, img="char2_new/normal", text="咦？怎麼什麼人都沒有？" , pos={x=320, y=15}},
  { ch=1, img="char2_new/normal", text="好奇怪呢...\n倒是眼前好像出現很多方塊..." },
  { ch=1, img="char2_new/normal", text="啊，已經是放學時間了呢。" },      
  { ch=1, text="" }
}

local act2_act3_ = {
  { ch=1, img="char2_new/normal", text="這裡有好多電腦，\n是電腦社呢。" , pos={x=320, y=15}},
  { ch=1, img="char2_new/normal", text="電腦螢幕跟電視長得好像，\n可是比起電腦，\n地味女更喜歡電視喔。"},
  { ch=1, img="char2_new/normal", text="因為看電視比較不需要動腦。"},
  { ch=2, img="char3_new/normal", text="哦...正好，直接現身了啊。",board_flip="HV" ,effect_a="slide_in", pos={x=490, y=300}},  
  { ch=2, img="char3_new/glad", text="剛剛的對戰我看到了，科科。",board_flip="V" },
  { ch=2, img="char3_new/glad", text="妳的倒數時間居然比較長，\n看來妳有控制時間的\n特殊屬性？" },
  { ch=1, img="char2_new/normal", text="啊？什麼？\n控制時間？\n你是說地味女會控制時間嗎？"},
  { ch=1, img="char2_new/normal", text="這麼說，\n我真的常常會有盯著窗外發呆，\n就覺得時間過得很慢的經驗..."},
  { ch=1, img="char2_new/angry", text="然後不小心睡著了，\n醒來已經是晚上，\n反而覺得時間過得超快呢。" , pos={x=320, y=70} },
  { ch=1, img="char2_new/glad", text="這就是你說的控制時間嗎？\n我都不知道這是特殊能力呢。" , pos={x=320, y=5} },
  { ch=2, img="char3_new/normal", text="呃不，那個應該不能算...",board_flip="V", pos={x=490, y=300} },  
  { ch=1, img="char2_new/angry", text="什麼嘛，\n是你先說\n地味女有控制時間能力的呢。" , pos={x=320, y=70} },
  { ch=2, img="char3_new/angry", text="跟妳無法溝通！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=420, y=230}},
  { ch=2, img="char3_new/sad", text="這一定有什麼誤會...", board="dialogue_bubble_1",board_flip="V" ,effect_w="size_M", pos={x=400, y=260}}, 
  { ch=1, img="char2_new/glad", text="耶～贏了～" },
  { ch=1, img="char2_new/glad", text="地味女，\n除了有時間控制能力之外，" },
  { ch=1, img="char2_new/glad", text="說不定還有\n「玩CuBeat很厲害」\n的能力呢！" },
  { ch=1, text="" }
}

local act2_act4_ = {
  { ch=1, img="char2_new/normal", text="哇～\n美術女的畫還是這麼漂亮呢。" , pos={x=320, y=15}},
  { ch=2, img="char4_new/glad", text="呵呵，是地味女啊。",board_flip="HV" ,effect_a="slide_in", pos={x=460, y=240} },
  { ch=2, img="char4_new/normal", text="雖然不知道妳是不是真的\n有品味來看出畫得是好是壞..."},
  { ch=2, img="char4_new/glad", text="不過本小姐的畫作\n可是從沒讓人失望過！"},
  { ch=1, img="char2_new/glad", text="耶～美術女好厲害～" , pos={x=320, y=5} },
  { ch=2, img="char4_new/normal", text="那麼，\n怎麼特地過來畫室這裡找我？"},
  { ch=1, img="char2_new/normal", text="是說啊，地味女覺得\n自己好像也變得很厲害呢，\n有特殊能力說！" , pos={x=320, y=15}},
  { ch=2, img="char4_new/glad", text="除了醒著跟睡著都在做夢之外\n妳還有什麼特殊能力？"},
  { ch=1, img="char2_new/glad", text="咦？有這個嗎？\n地味女突然\n又多了一個新的特殊能力了！" , pos={x=320, y=5} },
  { ch=2, img="char4_new/normal", text="...那是說妳整天都在\n發呆跟做白日夢的意思。"},
  { ch=1, img="char2_new/angry", text="哪有，\n地味女的能力很厲害的說！\n妳看！" , pos={x=320, y=70} },

  { ch=1, img="char2_new/glad", text="連美術女都贏了！耶～耶～" , pos={x=320, y=5} },
  { ch=2, img="char4_new/angry", text="才沒那回事！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=2, img="char4_new/sad", text="我、我是...\n因為妳是本小姐的朋友，\n才放水讓妳贏的！",effect_w="size_M" , board="dialogue_bubble_1", pos={x=520, y=240}},
  { ch=2, img="char4_new/sad", text="本小姐要繼續畫圖了，\n不要打擾我！"},
  { ch=1, text="" }
}

local act2_act5_ = {
  { ch=1, img="char2_new/normal", text="柔道社的道場樹木好多，\n好像叢林一樣喔。" , pos={x=320, y=15}},
  { ch=2, img="char5_new/glad", text="喝！呆毛小妹！\n你也是來鍛鍊身心的嗎？" ,board_flip="HV" ,effect_a="slide_in", pos={x=420, y=250} },
  { ch=1, img="char2_new/glad", text="唔哇！叢林野人出現了！" , pos={x=320, y=5} },
  { ch=2, img="char5_new/sad", text="叢林...野人...",board_flip="V", pos={x=420, y=230} },
  { ch=2, img="char5_new/normal", text="聽好了，\n在下是翔鯨武館館主，\n不是什麼叢林野人。" ,board_flip="V", pos={x=420, y=250}},
  { ch=1, img="char2_new/angry", text="耶～不好玩～\n如果出現的是叢林野人\n會比較有趣說。", pos={x=320, y=70}},
  { ch=2, img="char5_new/angry", text="不要管什麼叢林野人了！"},
  { ch=2, img="char5_new/glad", text="妳來這裡有何目的？\n是想要好好鍛練一番嗎？"},
  { ch=1, img="char2_new/normal", text="鍛鍊聽起來很累人呢。" , pos={x=320, y=15}},
  { ch=1, img="char2_new/angry", text="地位女討厭累人的事情。" , pos={x=320, y=70} },
  { ch=2, img="char5_new/angry", text="年輕人怎能如此慵懶！\n準備好應戰的覺悟吧！"},

  { ch=1, img="char2_new/glad", text="地位女很厲害吧！\n這樣我還需要鍛鍊嗎？" , pos={x=320, y=5} },
  { ch=2, img="char5_new/sad", text="...看來是在下需要上山\n找叢林野人鍛鍊了..." ,board_flip="V", pos={x=420, y=230}},
  { ch=1, text="" }
}

local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="是魔術社！\n地位女也很喜歡魔術呢。" , pos={x=320, y=5}},
  { ch=2, img="char6_new/normal", text="......" ,board_flip="HV" ,effect_a="slide_in", pos={x=470, y=250} },
  { ch=1, img="char2_new/normal", text="啊，妳是魔術社的魔術女。" , pos={x=320, y=15}},
  { ch=1, img="char2_new/normal", text="聽說妳很會變魔術呢，\n可以變給地味女看嗎？" , pos={x=320, y=15}},
  { ch=2, img="char6_new/glad", text="......嗯。", pos={x=440, y=250}},
  { ch=1, img="char2_new/glad", text="耶～是鴿子！" , pos={x=320, y=5}},
  { ch=1, img="char2_new/normal", text="地味女一直很好奇，\n為什麼魔術變出來的動物\n都是鴿子呢？" , pos={x=320, y=15}},
  { ch=1, img="char2_new/normal", text="我比較喜歡小貓說。"},
  { ch=2, img="char6_new/normal", text="......喔。"},
  { ch=1, img="char2_new/normal", text="對了，魔術女，\n變化方塊的CuBeat\n算是魔術的一種嗎？"},
  { ch=2, img="char6_new/normal", text="......不是。"},
  { ch=1, img="char2_new/angry", text="啊～失望。\n還以為我也學會了變魔術說。", pos={x=320, y=70}},
  { ch=2, img="char6_new/angry", text="哪那麼...簡單！", pos={x=400, y=300}},
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=1, img="char3_new/normal", text="嗯？",board_flip="V", pos={x=300, y=300}},
  { ch=2, img="char1_new/glad", text="啊哈！\n我要成為最強的\nCuBeat能力者！" ,board_flip="H",effect_a="slide_in", pos={x=384, y=70}},
  { ch=1, img="char3_new/normal", text="喂，那邊那個紅頭髮的。" },
  { ch=2, img="char1_new/normal", text="嗨，有事嗎？" , pos={x=440, y=15}},
  { ch=1, img="char3_new/glad", text="你手上那方塊型護腕...\n挺特別的，科科。",board_flip="V", pos={x=330, y=260} },
  { ch=2, img="char1_new/glad", text="是啊，這很酷吧！\n不過跟方塊有關的事情\n我還有個更酷的...", pos={x=384, y=70}},
  { ch=1, img="char3_new/normal", text="方塊操控能力對吧。" , pos={x=300, y=300},board_flip="V"},
  { ch=2, img="char1_new/angry", text="是CuBeat能力！我取名的！\n不過你怎麼會知道這個？" , pos={x=400, y=30}},
  { ch=1, img="char3_new/glad", text="名字取得不錯，\n至於我怎麼知道的那不重要，\n重要的是..." ,board_flip="V", pos={x=330, y=260}},
  { ch=2, img="char1_new/glad", text="你也會CuBeat對吧！\n那就來較量一下吧！", pos={x=384, y=70} },
  { ch=1, img="char3_new/angry", text="正合我意！" ,board="dialogue_bubble_2", pos={x=400, y=260}},
  
  { ch=2, img="char1_new/sad", text="怎麼會..." , pos={x=410, y=70}},
  { ch=1, img="char3_new/glad", text="下次跟人宣戰前先想清楚，\n講話別那麼急啊。",board="dialogue_bubble_1", pos={x=330, y=260} },
  { ch=1, img="char3_new/glad", text="不過就算你沒跟我宣戰\n我也會這麼做的，科科。" },
  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=1, img="char3_new/normal", text="有情報說地味女\n那個呆呆女也懂得控制方塊。",board_flip="V", pos={x=300, y=300}},
  { ch=1, img="char3_new/glad", text="看來智商高低並不是\n能不能控制方塊的條件。",board_flip="V", pos={x=330, y=260}},
  { ch=1, img="char3_new/normal", text="喂，地味女。\n有事情要問你。",board_flip="V", pos={x=300, y=300}},
  { ch=2, img="char2_new/normal", text="是喔，\n可是地味女現在頭昏昏的，\n滿腦子都是奇怪的方塊說。",board_flip="H", pos={x=430, y=15},effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="被方塊打到頭？\n放心吧，不用怕會變笨。"},
  { ch=1, img="char3_new/normal", text="因為妳很難再變得更笨了。"},
  { ch=2, img="char2_new/angry", text="耶～好過分！", pos={x=430, y=15}},
  { ch=2, img="char2_new/normal", text="地味女現在\n會一個很特殊的能力喔！\n我可以控制這些方塊！"},
  { ch=1, img="char3_new/glad", text="科科，果然沒錯，\n妳也具有控制方塊的能力。",board_flip="V", pos={x=330, y=260}},
  { ch=2, img="char2_new/angry", text="該不會跟正太男一樣，\n又要跟地味女對戰了吧。"},
  { ch=2, img="char2_new/glad", text="不過這好像代表\n地味女很受歡迎呢！", pos={x=430, y=5}},
  { ch=1, img="char3_new/normal", text="是啊。",board_flip="V", pos={x=300, y=300}},
  { ch=1, img="char3_new/glad", text="很不錯的靶子。",board_flip="V", pos={x=330, y=260}},

  { ch=2, img="char2_new/sad", text="你們都欺負地位女啦～", pos={x=430, y=15}},
  { ch=1, img="char3_new/glad", text="剛剛有說到正太男對吧，\n下一個目標鎖定！"},
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="今天又有哪些事情發生呢？\n科科科。", pos={x=330, y=260}},
  { ch=1, img="char3_new/glad", text="我建立的校內情報交換系統\n總是能為我帶來些樂子呢。"},
  { ch=1, img="char3_new/glad", text="我看看..."},
  { ch=1, img="char3_new/glad", text="「傳說學校吉祥物飛天鯨魚\n最喜歡吃的食物是洋芋片！」"},
  { ch=1, img="char3_new/glad", text="「其實這間學校的校長\n是外星小藍人！」"},
  { ch=1, img="char3_new/angry", text="怎麼都是些\n莫名其妙的八卦！" ,effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=400, y=260}},
  { ch=1, img="char3_new/normal", text="嗯？這則情報是什麼？", board="dialogue_bubble_1", board_flip="V", effect_w="size_M", pos={x=300, y=300}},
  { ch=1, img="char3_new/normal", text="「校園內出現大量神秘方塊，\n這些方塊的來源與作用不明..."},
  { ch=1, img="char3_new/normal", text="有目擊者指出，\n某些人具有使用這些方塊\n來進行對戰的能力...」"},  
  { ch=1, img="char3_new/glad", text="有趣，控制方塊是吧？" ,board_flip="V", pos={x=330, y=260}},
  { ch=1, img="char3_new/glad", text="正好，\n確實看到了幾個這樣的方塊，\n讓我來試試，科科。"},
  { ch=1, img="char3_new/glad", text="很好，再來就是調查一下\n有方塊控制能力的是哪些人..."},
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=2, img="char4_new/normal", text="嗯？\n電腦社的，來這有什麼事？",board_flip="H" ,effect_a="slide_in", pos={x=460, y=-15}},
  { ch=1, img="char3_new/glad", text="這個嘛，不過是感覺到\n這裡有點不尋常而已，\n科科。", pos={x=330, y=260}},
  { ch=2, img="char4_new/glad", text="怎麼？\n你現在才發現\n本小姐的畫技並不尋常嗎？"},
  { ch=1, img="char3_new/normal", text="不是。\n我不是在說這個。",board_flip="V", pos={x=300, y=300}},
  { ch=2, img="char4_new/glad", text="呵呵，\n本小姐的畫也確實不是\n尋常價位買得下來的呢。"},
  { ch=1, img="char3_new/normal", text="麻煩先聽我說話啊。喂。", pos={x=300, y=300}},
  { ch=2, img="char4_new/glad", text="如果是太過尋常的讚美\n本小姐可不會接受喔。"},
  { ch=1, img="char3_new/angry", text="無法溝通！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=400, y=260}},
  
  { ch=1, img="char3_new/glad", text="跟我推測的一樣，\n妳果然有控制方塊的能力。",board_flip="V",effect_w="size_M",board="dialogue_bubble_1", pos={x=330, y=260}},
  { ch=1, img="char3_new/glad", text="只是技巧還有待加強就是，\n科科。"},
  { ch=2, img="char4_new/sad", text="...你這種讚美方式\n本小姐聽不懂...。", pos={x=500, y=-25}},
  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="差不多可以推測出\nCuBeat能力的獲得條件了。",board_flip="V", pos={x=300, y=300}},
  { ch=1, img="char3_new/normal", text="美術女跟地味女的髮飾、\n正太男的護腕..." },
  { ch=1, img="char3_new/normal", text="只要身上有著方塊型配件、\n再去碰觸到方塊..." },
  { ch=1, img="char3_new/glad", text="就會得到CuBeat能力！",board_flip="V", pos={x=330, y=260}},
  { ch=1, img="char3_new/glad", text="下一個可能符合條件的，\n是這個腰帶有顆方塊的傢伙。" },
  { ch=2, img="char5_new/normal", text="喔？電腦少年，\n想來鍛鍊身心嗎？" ,board="dialogue_bubble_3",board_flip="H", pos={x=420, y=40},effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="沒興趣。",board_flip="V", pos={x=300, y=330}},
  { ch=2, img="char5_new/glad", text="不要害羞！\n吾看你電腦不離手，\n一定缺乏運動吧！",board_flip="V", pos={x=440, y=140}},
  { ch=1, img="char3_new/normal", text="真不想聽\n頭腦裡也長肌肉的人說教。" },
  { ch=2, img="char5_new/glad", text="比起電腦，\n更重要的是你的頭腦！\n由你的頭腦直接連結的身心！"},
  { ch=1, img="char3_new/angry", text="煩死了！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=400, y=260}},
  { ch=1, img="char3_new/normal", text="現在是誰的頭腦\n比較需要鍛鍊啊？",board_flip="V",effect_w="size_M",board="dialogue_bubble_1", pos={x=300, y=70}},
  { ch=2, img="char5_new/sad", text="......" ,board="dialogue_bubble_1",board_flip="V", pos={x=420, y=250}},
  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/normal", text="最新情報是魔術社\n也有人會玩CuBeat。",board_flip="V", pos={x=300, y=330}},
  { ch=1, img="char3_new/normal", text="我記得魔術社有個\n很會變魔術\n但是不善言辭的女魔術師...。"},
  { ch=2, img="char6_new/glad", text="......嘿。",board_flip="HV", pos={x=470, y=160},effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="不錯不錯，\n想必我要找的人就是妳了。",board_flip="V", pos={x=330, y=330}},
  { ch=2, img="char6_new/normal", text="......？", pos={x=500, y=160}},
  { ch=1, img="char3_new/normal", text="等等，妳的身上\n好像沒有方塊型的配件。",board_flip="V", pos={x=300, y=330}},
  { ch=1, img="char3_new/normal", text="不是CuBeat能力者嗎...？"},
  { ch=2, img="char6_new/angry", text="哼！",board_flip="V", pos={x=400, y=60}},
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
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown_[k]
end


return {
  get_rundown = get_rundown
}