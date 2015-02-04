local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[

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
  { ch=1, img="char1_new/glad", text="下課啦下課啦！" ,effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="今天真是好天氣！\n熱血滿滿的來去練球啦！" },
  { ch=1, img="char1_new/normal", text="咦，等等，那是什麼？"},
  { ch=1, img="char1_new/sad", text="痛痛痛…" , board="dialogue_bubble_2", pos={x=380, y=180}, effect_w="shake", special={id="cube", x1=500, y1=-50, x2=350, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char1_new/sad", text="好像被什麼東西打到了…", effect_w="size_M", board="dialogue_bubble_1" },
  { ch=1, img="char1_new/anger", text="喂！\n這些方塊是怎麼回事！？\n看我把你們全部打回天上去！" },
  { ch=1, text="" }
}

local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="嘿～！雅季～！\n還在教室嗎？",effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="妳已經從上課時間睡到放學囉！" },
  { ch=2, img="char2_new/normal", text="……。" , board="dialogue_bubble_1" ,board_flip="HV"},
  { ch=1, img="char1_new/glad", text="果然還在教室啊！\n睡夠了吧？跟妳說跟妳說…" },
  { ch=2, img="char2_new/normal", text="……。" },
  { ch=1, img="char1_new/normal", text="咦？沒聽到嗎？" },
  { ch=1, img="char1_new/anger", text="喂～！雅季！" ,pos={x=350, y=150}, board="dialogue_bubble_2", effect_w="shake"},
  { ch=2, img="char2_new/normal", text="啊啊？是嵐泰啊…",effect_a="shake"},
  { ch=1, img="char1_new/normal", text="妳在發什麼呆啊？" , board="dialogue_bubble_1",effect_w="size_M" },
  { ch=2, img="char2_new/normal", text="覺得頭昏昏的說…"},
  { ch=2, img="char2_new/sad", text="好像被什麼東西打到頭的感覺，\n腦袋裡都是方塊的畫面，"},
  { ch=2, img="char2_new/anger", text="而且只要雅季用力想，\n那些方塊就會隨我想的消失。"},
  { ch=1, img="char1_new/anger", text="喂喂…\n難道說妳也得到了CuBeat能力？"},
  { ch=2, img="char2_new/normal", text="ＱＢ？那是什麼？"},
  { ch=1, img="char1_new/anger", text="雅季！一決勝負吧！" , board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=2, img="char2_new/normal", text="咦咦！？" },
  { ch=1, img="char1_new/anger", text="看看誰才是最會CuBeat的人！", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}

local act1_act3_ = {
  { ch=1, img="char1_new/normal", text="喔～\n電腦社的設備看起來很厲害呢。",effect_a="slide_in" },
  { ch=2, img="char3_new/normal", text="要幹嘛？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="啊，沒什麼啦，\n只是想問看看…" },
  { ch=1, img="char1_new/normal", text="你知道嗎？\n學校出現很多不明方塊呢。" },
  { ch=2, img="char3_new/normal", text="哼，\n你想挑戰我的情報收集能力嗎？"},
  { ch=1, img="char1_new/normal", text="這麼說你是知道囉！\n有些人有控制方塊的能力！"},
  { ch=1, img="char1_new/glad", text="那種能力叫做「CuBeat」，\n是我取的名字！"},
  { ch=2, img="char3_new/glad", text="「CuBeat」是嗎？",board_flip="V"},
  { ch=2, img="char3_new/glad", text="你想知道還有哪些人也擁有你所說\n的CuBeat能力？"},
  { ch=2, img="char3_new/glad", text="哼哼…科科…"},
  { ch=2, img="char3_new/anger", text="我也有啊！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act1_act4_ = {
  { ch=1, img="char1_new/normal", text="美術室…真是壯觀啊。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="齊格說這裡可能有人會CuBeat，\n不知道是不是真的？" },
  { ch=2, img="char4_new/normal", text="喂，你幹嘛？\n不要打擾本小姐作畫。",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="作畫？\n這些圖都是妳畫的嗎？" }, 
  { ch=1, img="char1_new/anger", text="好漂亮！\n真是太厲害了！"},
  { ch=2, img="char4_new/glad", text="呵，可不要以為這種程度的誇獎就\n能夠討本小姐歡心啊。"}, 
  { ch=1, img="char1_new/glad", text="那妳一定也有CuBeat能力吧！"},
  { ch=1, img="char1_new/glad", text="就是能夠控制方塊，\n讓方塊排列消除的能力！"},
  { ch=2, img="char4_new/normal", text="本小姐聽不懂你在說什麼。"},
  { ch=1, img="char1_new/sad", text="耶？\n難道齊格給的是假情報？"},
  { ch=1, img="char1_new/sad", text="真讓人失望…。"},
  { ch=2, img="char4_new/anger", text="無禮！沒有人可以\n對本小姐失望！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  
  { ch=1, text="" }
}

local act1_act5_ = {
  { ch=1, img="char1_new/normal", text="翔鯨武館…\n不知不覺就走到這裡了。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="傳說中的阿廣學長不知道在不在？" },
  { ch=1, img="char1_new/glad", text="如果他真有傳說中那麼厲害，\n那只要贏過他我就是最強了！"},
  { ch=2, img="char5_new/normal", text="哈！棒球小弟！\n你也是來鍛鍊身心的嗎？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="鍛鍊身心？"},
  { ch=1, img="char1_new/normal", text="不是啦，\n我是在找跟我一樣有方塊消除能力\n的人。" },
  { ch=2, img="char5_new/glad", text="喝！\n那棒球小弟你沒有找錯地方！"},
  { ch=1, img="char1_new/anger", text="果然如此！" },
  { ch=1, img="char1_new/anger", text="學長！\n跟我一決勝負吧！" },
  { ch=1, img="char1_new/anger", text="如果能贏過你，\n我就能成為最強！" },
  { ch=1, img="char1_new/normal", text="…我有這種感覺。"},
  { ch=2, img="char5_new/normal", text="想成為最強是嗎？"},
  { ch=2, img="char5_new/anger", text="棒球小弟，\n你可曾想過什麼樣才叫做最強！？"},
  { ch=2, img="char5_new/normal", text="成為了最強之後，\n下一步又是什麼？"},
  { ch=1, img="char1_new/anger", text="當然是…", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=360, y=100} },
  { ch=1, img="char1_new/anger", text="是…" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char1_new/normal", text="對耶，這我好像沒想過。"},
  { ch=2, img="char5_new/anger", text="給我用用頭腦啊！\n年輕人！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=380, y=80} },
  { ch=1, img="char1_new/sad", text="你先打贏我再跟我說教吧…"},
  { ch=2, img="char5_new/glad", text="那在下可不會手下留情！" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, text="" }
}

local act1_act6_ = {
  { ch=1, img="char1_new/normal", text="CuBeat這種不可思議的事情，\n說起來還真像是魔術。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="魔術社的人會不會知道些什麼呢？"},
  { ch=1, img="char1_new/glad", text="哈囉，魔術社的各位大家好！" },
  { ch=2, img="char6_new/normal", text="……。" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="那個…妳好？"},
  { ch=2, img="char6_new/glad", text="……。"},
  { ch=1, img="char1_new/glad", text="喔喔！好厲害！\n不愧是魔術師，\n好有趣的打招呼方式！"},
  { ch=2, img="char6_new/normal", text="……嗯。"},
  { ch=1, img="char1_new/normal", text="妳真的很會變魔術呢。" },
  { ch=1, img="char1_new/normal", text="那麼有種關於方塊的「魔術」，\n妳應該也沒問題吧？"},
  { ch=2, img="char6_new/anger", text="哼！！"},
  { ch=1, img="char1_new/anger", text="耶？突然就宣戰！？" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/normal", text="啊，是嵐泰～",effect_a="slide_in"},
  { ch=2, img="char1_new/glad", text="雅季！妳來得正好！\n給妳看件很有趣的事情！"  ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="哇～真的嗎？\n是什麼呢？\n長得像兔子的小貓嗎？" ,board_flip="V"},
  { ch=2, img="char1_new/normal", text="絕對不是。\n那個有趣嗎？"},
  { ch=1, img="char2_new/anger", text="有的話一定很有趣～"},
  { ch=2, img="char1_new/anger", text="不要管什麼阿貓阿狗了！\n來試試這個吧！"},
  
  { ch=1, text="" }
}

local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="今天的天氣很不錯呢～"},
  { ch=1, img="char2_new/normal", text="有種可以盯著窗外看天空一整天的\n感覺。" },
  { ch=1, img="char2_new/normal", text="這樣很幸福的說。" },
  { ch=1, img="char2_new/sad", text="好痛！" ,effect_w="shake", board="dialogue_bubble_2", pos={x=370, y=120}, special={id="cube", x1=500, y1=-50, x2=300, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char2_new/anger", text="討厭！誰偷打人家的頭！", effect_w="size_M", board="dialogue_bubble_1" ,board_flip="V" },
  { ch=1, img="char2_new/normal", text="咦？怎麼什麼人都沒有？",board_flip="V"},
  { ch=1, img="char2_new/normal", text="好奇怪呢…\n倒是眼前好像出現很多方塊…" },
   
  { ch=1, text="" }
}

local act2_act3_ = {
  { ch=1, img="char2_new/normal", text="這裡有好多電腦，\n是電腦社呢。" ,effect_a="slide_in"},
  { ch=1, img="char2_new/normal", text="電腦螢幕跟電視長得好像，\n可是比起電腦，\n雅季更喜歡電視喔。"},
  { ch=1, img="char2_new/normal", text="因為看電視比較不需要動腦。"},
  { ch=2, img="char3_new/normal", text="哦…正好，直接現身了啊。",board_flip="HV" ,effect_a="slide_in"},  
  { ch=2, img="char3_new/glad", text="剛剛的對戰我看到了，科科。",board_flip="V"},
  { ch=2, img="char3_new/glad", text="妳的倒數時間居然比較長，\n看來妳有控制時間的特性？"},
  { ch=1, img="char2_new/normal", text="啊？什麼？\n控制時間？\n你是說雅季會控制時間嗎？"},
  { ch=1, img="char2_new/normal", text="這麼說，\n雅季真的常常有盯著窗外發呆，\n就覺得時間過得很慢的經驗…"},
  { ch=1, img="char2_new/anger", text="然後不小心睡著了，\n醒來已經是晚上，\n反而覺得時間過得超快呢。" ,board_flip="V"},
  { ch=1, img="char2_new/glad", text="這就是你說的控制時間嗎？\n我都不知道這是特殊能力呢。"},
  { ch=2, img="char3_new/normal", text="呃不，那個應該不能算…",board_flip="V"},  
  { ch=1, img="char2_new/anger", text="什麼嘛，\n是你先說雅季有控制時間能力的呢。"},
  { ch=2, img="char3_new/anger", text="無法溝通！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act2_act4_ = {
  { ch=1, img="char2_new/normal", text="哇～\n俐亞的畫還是這麼漂亮呢。" ,effect_a="slide_in"},
  { ch=2, img="char4_new/glad", text="嗯？是雅季啊。",board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="雖然不知道妳是不是真的有品味來\n看出畫得是好是壞…"},
  { ch=2, img="char4_new/glad", text="不過本小姐的畫作可是從沒讓人失\n望過！"},
  { ch=1, img="char2_new/glad", text="耶～俐亞好厲害～",board_flip="V"},
  { ch=2, img="char4_new/normal", text="那麼，\n怎麼特地過來畫室這裡找我？"},
  { ch=1, img="char2_new/normal", text="是說啊，\n雅季覺得自己好像也變得很厲害呢，\n有特殊能力說！" ,board_flip="V"},
  { ch=2, img="char4_new/glad", text="除了醒著跟睡著都在做夢之外，\n妳還有什麼特殊能力？"},
  { ch=1, img="char2_new/glad", text="咦？有這個嗎？\n雅季突然又多了一個新的特殊能力！" ,board_flip="V"},
  { ch=2, img="char4_new/normal", text="是說妳整天都在發呆跟做白日夢的\n意思。"},
  { ch=1, img="char2_new/anger", text="哪有，\n雅季的能力很厲害的說！\n妳看！" },
  
  { ch=1, text="" }
}

local act2_act5_ = {
  { ch=1, img="char2_new/normal", text="柔道社的道場樹木好多，\n好像叢林一樣喔。",effect_a="slide_in"},
  { ch=2, img="char5_new/glad", text="喝！呆毛小妹！\n你也是來鍛鍊身心的嗎？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="唔哇！叢林野人出現了！",board_flip="V"},
  { ch=2, img="char5_new/sad", text="叢林…野人…",board_flip="V"},
  { ch=2, img="char5_new/normal", text="聽好了，\n在下是翔鯨武館館主，\n不是什麼叢林野人。" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="耶～不好玩～\n如果出現的是叢林野人會比較有趣\n說。",},
  { ch=2, img="char5_new/anger", text="不要管什麼叢林野人了！"},
  { ch=2, img="char5_new/glad", text="妳來這裡有何目的？\n是想要好好鍛鍊一番嗎？"},
  { ch=1, img="char2_new/normal", text="鍛鍊聽起來很累人呢。" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="雅季討厭累人的事情。" ,board_flip="V"},
  { ch=2, img="char5_new/anger", text="年輕人怎能如此慵懶！\n準備好應戰的覺悟吧！"},
  
  { ch=1, text="" }
}

local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="是魔術社！\n雅季也很喜歡魔術呢。" ,board_flip="V"},
  { ch=2, img="char6_new/normal", text="……。" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/normal", text="啊，妳是魔術社的雪青。" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="聽說妳很會變魔術呢，\n可以變給雅季看嗎？"},
  { ch=2, img="char6_new/glad", text="……嗯。"},
  { ch=1, img="char2_new/glad", text="耶～是鴿子！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="其實雅季比較喜歡小貓說。",board_flip="V"},
  { ch=2, img="char6_new/normal", text="……喔。"},
  { ch=1, img="char2_new/normal", text="對了，雪青，\n變化方塊的CuBeat算是魔術的一種\n嗎？"},
  { ch=2, img="char6_new/normal", text="……不是。"},
  { ch=1, img="char2_new/anger", text="啊～失望。\n還以為我也學會了變魔術說。",board_flip="V"},
  { ch=2, img="char6_new/anger", text="哪那麼…簡單！"},
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=1, img="char3_new/normal", text="嗯？",effect_a="slide_in"},
  { ch=2, img="char1_new/glad", text="啊哈！\n我要成為最強的CuBeat能力者！" ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="喂，那邊那個紅頭髮的。" },
  { ch=2, img="char1_new/normal", text="嗯？找我嗎？"},
  { ch=1, img="char3_new/glad", text="你手上那方塊型護腕…\n挺特別的，科科。"},
  { ch=2, img="char1_new/glad", text="是啊，這很酷吧！\n不過跟方塊有關的事情我還有個更\n酷的…"},
  { ch=1, img="char3_new/normal", text="方塊操控能力對吧。"},
  { ch=2, img="char1_new/anger", text="是CuBeat能力！我取名的！"},
  { ch=2, img="char1_new/normal", text="不過你怎麼會知道？"}, 
  { ch=1, img="char3_new/glad", text="名字取得不錯，\n至於我怎麼知道的那不重要，\n重要的是…"},
  { ch=2, img="char1_new/glad", text="你也會CuBeat對吧！\n那就來較量一下吧！"},
  { ch=1, img="char3_new/anger", text="正合我意！", effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=1, img="char3_new/normal", text="有情報說雅季那個呆呆女也具有控\n制方塊能力。",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="看來智商高低不是能不能控制方塊\n的條件。"},
  { ch=1, img="char3_new/normal", text="喂，雅季。\n有事情要問你。"},
  { ch=2, img="char2_new/normal", text="是喔，\n可是雅季現在頭昏昏的說。",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="被方塊打到頭？\n放心吧，不用怕會變笨。"},
  { ch=1, img="char3_new/normal", text="因為妳很難再變得更笨了。"},
  { ch=2, img="char2_new/anger", text="耶～好過分！"},
  { ch=2, img="char2_new/normal", text="雅季現在會一個很特殊的能力喔！\n可以控制方塊的說！"},
  { ch=1, img="char3_new/glad", text="科科，果然沒錯，\n妳也具有控制方塊的能力。"},
  { ch=2, img="char2_new/anger", text="該不會跟嵐泰一樣，\n又要跟雅季對戰了吧。"},
  { ch=2, img="char2_new/glad", text="不過這好像代表雅季很受歡迎呢！"},
  { ch=1, img="char3_new/normal", text="是啊。"},
  { ch=1, img="char3_new/glad", text="很不錯的靶子。"},
  
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="今天又有哪些事情發生呢？\n科科科。"},
  { ch=1, img="char3_new/glad", text="我建立的校內情報交換系統總是能\n為我帶來些樂子呢。"},
  { ch=1, img="char3_new/glad", text="我看看…"},
  { ch=1, img="char3_new/glad", text="「傳說學校吉祥物飛天鯨魚最喜歡\n吃的食物是洋芋片！」"},
  { ch=1, img="char3_new/glad", text="「其實這間學校的校長是外星小藍\n人！」"},
  { ch=1, img="char3_new/anger", text="怎麼都是些\n莫名其妙的八卦！" ,effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=360, y=260}},
  { ch=1, img="char3_new/normal", text="嗯？這則情報是什麼？", board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char3_new/normal", text="「校園內出現大量神秘方塊，\n這些方塊的來源與作用不明…"},
  { ch=1, img="char3_new/normal", text="有目擊者指出，\n某些人有能力控制這些方塊來進行\n對戰…」"},  
  { ch=1, img="char3_new/glad", text="有趣，控制方塊是吧？"},
  { ch=1, img="char3_new/glad", text="正好，\n確實看到了幾個這樣的方塊，\n讓我來試試，科科。"},
  
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=2, img="char4_new/normal", text="嗯？\n電腦社的，有什麼事？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="這個嘛，\n不過是感覺到這裡有點不尋常而已，\n科科。",effect_a="slide_in"},
  { ch=2, img="char4_new/glad", text="怎麼？\n現在才發現本小姐的畫技非比尋常\n嗎？"},
  { ch=1, img="char3_new/normal", text="不是。\n我不是在說這個。"},
  { ch=2, img="char4_new/glad", text="呵呵，\n本小姐的畫也確實不是尋常價位所\n能夠買下來的呢。"},
  { ch=1, img="char3_new/normal", text="麻煩先聽我說話啊。喂。"},
  { ch=2, img="char4_new/glad", text="如果是太過尋常的讚美本小姐可是\n不會接受喔。"},
  { ch=1, img="char3_new/anger", text="無法溝通！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=360, y=260}},

  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="差不多可以推測出CuBeat能力的\n獲得條件了。"},
  { ch=1, img="char3_new/normal", text="俐亞跟雅季的髮飾、\n嵐泰的護腕…" },
  { ch=1, img="char3_new/normal", text="只要身上有著方塊型配件、\n再去碰觸到方塊…" },
  { ch=1, img="char3_new/glad", text="就會得到CuBeat能力！"},
  { ch=1, img="char3_new/glad", text="下一個可能符合條件的，\n是這個腰帶有顆方塊的傢伙。" },
  { ch=2, img="char5_new/normal", text="喔？電腦少年，\n想來鍛鍊身心嗎？",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="沒興趣。"},
  { ch=2, img="char5_new/glad", text="不要害羞！\n看你電腦不離手，\n一定缺乏運動吧！"},
  { ch=1, img="char3_new/normal", text="真不想聽頭腦裡也長肌肉的人說教。" },
  { ch=2, img="char5_new/glad", text="比起電腦，\n更重要的是你的頭腦！\n由你的頭腦直接連結的身心！"},
  { ch=1, img="char3_new/anger", text="煩死了！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/normal", text="最新情報是魔術社也有人會玩\nCuBeat。",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="我記得魔術社有個很會變魔術但是\n不太會說話的女魔術師…。"},
  { ch=2, img="char6_new/glad", text="……嘿。",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="不錯不錯，\n想必我要找的人就是妳了。"},
  { ch=2, img="char6_new/normal", text="……？"},
  { ch=1, img="char3_new/normal", text="等等，\n妳的身上好像沒有方塊型的配件。"},
  { ch=1, img="char3_new/normal", text="不是CuBeat能力者嗎…？"},
  { ch=2, img="char6_new/anger", text="哼！"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/normal", text="喔？",effect_a="slide_in"},
  { ch=2, img="char1_new/glad", text="找到了～我正在找妳呢！",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="呵，\n看來本小姐的名聲確實廣傳了呢。"},
  { ch=2, img="char1_new/anger", text="我正在到處找CuBeat能力者呢！"},
  { ch=2, img="char1_new/glad", text="聽說妳有CuBeat能力，\n快來對戰吧！"},
  { ch=1, img="char4_new/normal", text="CuBeat？那是什麼？\n本小姐沒聽說過。"},
  { ch=2, img="char1_new/normal", text="對喔，因為名字是我剛取的。"},
  { ch=1, img="char4_new/normal", text="感覺很隨便呢，本小姐沒興趣。"},
  { ch=2, img="char1_new/anger", text="那就隨便跟我比一場吧！"},
  
  { ch=1, text="" }
}

local act4_act2_ = {
  { ch=1, img="char4_new/normal", text="還在教室啊，雅季？\n已經是下課時間囉。",effect_a="slide_in"},
  { ch=2, img="char2_new/normal", text="啊……。",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="別發呆了，\n來看看本小姐剛甦醒的美妙才能吧！"},
  { ch=2, img="char2_new/glad", text="美喵？聽起來就很可愛呢！"},
  { ch=1, img="char4_new/normal", text="不是美喵，我說的是美妙。"},
  { ch=2, img="char2_new/glad", text="美喵～美喵～\n雅季最喜歡貓了。"}  ,
  { ch=1, img="char4_new/anger", text="別鬧了，給我認真點！"} ,

  { ch=1, text="" }
}


local act4_act3_ = {
  { ch=1, img="char4_new/normal", text="聽說電腦社在搞什麼校園情報網站。",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="該是讓更多人認識本小姐的時候了！"},
  { ch=2, img="char3_new/normal", text="有何貴幹？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="能夠讓本小姐大駕光臨，\n真該說你們運氣不錯呢。"},
  { ch=2, img="char3_new/normal", text="喔。"},  
  { ch=1, img="char4_new/normal", text="是說你們這房間可真是缺乏美感啊。"},
  { ch=1, img="char4_new/normal", text="需不需要本小姐畫幾幅畫讓你們掛\n牆上？"},
  { ch=1, img="char4_new/glad", text="當然不可能是免費的就是了，\n呵呵。"},
  { ch=2, img="char3_new/normal", text="所以妳到底是來做什麼的？"},
  { ch=1, img="char4_new/normal", text="看來你還沒聽說過本小姐的名號。"},
  { ch=2, img="char3_new/glad", text="我不會收集沒用的資訊。",board_flip="V" },
  { ch=1, img="char4_new/anger", text="何等無禮！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=300, y=100}},
  
  { ch=1, text="" }
}

local act4_act4_ = {
  { ch=1, img="char4_new/normal", text="很好，這樣這幅畫就完成了。",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="簡直完美呢，本小姐的畫作。"},
  { ch=1, img="char4_new/sad", text="咿！",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=170, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},board="dialogue_bubble_2",board_flip="V", pos={x=300, y=100} },
  { ch=1, img="char4_new/anger", text="這些方塊是什麼！？",effect_w="size_M",board="dialogue_bubble_1",board_flip="V"},

  { ch=1, text="" }
}

local act4_act5_ = {
  { ch=1, img="char4_new/glad", text="歡呼吧！柔道社社員！\n本小姐前來造訪了！" ,effect_a="slide_in"},
  { ch=2, img="char5_new/normal", text="喔？美術少女，\n妳也想來鍛鍊身心嗎？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char5_new/anger", text="可惜！\n在下正在探究一個難題，\n可能沒有時間奉陪。"},
  { ch=1, img="char4_new/anger", text="要知道本小姐親自登門是多大榮幸！\n可沒有人敢拒絕本小姐！",effect_a="shake"},
  { ch=2, img="char5_new/normal", text="這倒是失禮了。"},
  { ch=2, img="char5_new/normal", text="美術少女妳可知道，\n校園內出現了許多神秘方塊？"},
  { ch=2, img="char5_new/anger", text="在下正在研究這到底是怎麼回事？"},
  { ch=2, img="char5_new/normal", text="嗯…在下有預感…"},
  { ch=2, img="char5_new/glad", text="在此時特地找上門的妳，\n應該也有這種能力。" },
  { ch=1, img="char4_new/glad", text="啊啦，這就是正中直拳嗎？\n可別後悔喔。"},
  
  { ch=1, text="" }
}

local act4_act6_ = {
  { ch=1, img="char4_new/normal", text="嗯，是魔術社啊。",effect_a="slide_in"},
  { ch=2, img="char6_new/glad", text="嘿…。",board_flip="HV"},
  { ch=1, img="char4_new/normal", text="不用出來迎接沒關係喔，\n本小姐只是剛好路過而已。"},
  { ch=2, img="char6_new/normal", text="……。"},
  { ch=1, img="char4_new/glad", text="算了，\n本小姐今天心情好，妳運氣不錯。"},
  { ch=1, img="char4_new/glad", text="就順便讓妳看看本小姐的畫筆有多\n大的能量吧！"},
  { ch=2, img="char6_new/anger", text="哼！"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2, img="char1_new/glad" ,text="嘿～大家～\n有沒有人要玩CuBeat啊！" ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char5_new/normal" ,text="CuBeat？\n喝！棒球小弟！\n你所說的CuBeat是指何事？",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/normal" ,text="是個超有趣又不可思議的能力呢。" },
  { ch=2, img="char1_new/glad" ,text="可以用它來消除學校內突然出現的\n不明方塊！" },
  { ch=1, img="char5_new/glad" ,text="看來，\n棒球小弟也遇上了跟在下一樣的事！" },
  { ch=2, img="char1_new/anger" ,text="咦！\n你不是傳說中的阿廣學長嗎？" },
  { ch=2, img="char1_new/glad" ,text="學長也有CuBeat能力嗎？\n來跟我進行一場對戰吧！" },
  { ch=1, img="char5_new/normal" ,text="在那之前，我想先問問棒球小弟，\n你是如何得到這樣的能力？" },
  { ch=2, img="char1_new/glad" ,text="突然就會了！很厲害吧！" },
  { ch=1, img="char5_new/normal" ,text="難道棒球小弟你完全沒有想過，\n為什麼會獲得這樣的能力嗎？" },
  { ch=2, img="char1_new/normal" ,text="沒有耶。" },
  { ch=1, img="char5_new/anger" ,text="用用你的頭腦啊！年輕人！" },
  { ch=1, img="char5_new/glad" ,text="思考何為力量的來源！\n思考何為力量的責任！" },
  { ch=2, img="char1_new/anger" ,text="我想到了！" },
  { ch=2, img="char1_new/glad" ,text="大概因為我是個天才吧！" },
  { ch=1, img="char5_new/normal" ,text="棒球小弟…\n擁有自信不是一件壞事。"},
  { ch=1, img="char5_new/anger" ,text="但你應該要知道\n何為力量的謙卑！" ,board="dialogue_bubble_2",effect_w="shake"},
  
  { ch=1, text="" }
}

local act5_act2_ = {
  { ch=1, img="char5_new/normal" ,text="線索是有了，但謎團依然未解啊。" ,board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" ,text="啊，你好～要來玩CuBeat嗎？" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="出現在面前的人都是CuBeat能力者\n這種事，也許在下也該見怪不怪了。" },
  { ch=1, img="char5_new/glad" ,text="喝！在那之前，\n在下想先詢問呆毛小妹一件事！" },
  { ch=1, img="char5_new/anger" ,text="妳可曾想過眾人進行CuBeat對戰的\n目的為何？" },
  { ch=2, img="char2_new/normal" ,text="嗯～人家沒想過耶～\n我想想喔～" },
  { ch=2, img="char2_new/glad" ,text="大概是集滿七場勝利就能招喚出神，\n然後可以實現一個願望吧？" },
  { ch=1, img="char5_new/anger" ,text="這遊戲才沒有收集要素！\n又不是七○珠！" },
  { ch=2, img="char2_new/sad" ,text="不然就是…玩這個遊戲的人，\n如果不繼續贏下去就會死。" },
  { ch=1, img="char5_new/sad" ,text="未免也太恐怖！\n妳當這是什麼被詛咒的遊戲嗎！？" ,board_flip="V"},
  { ch=2, img="char2_new/anger" ,text="阿廣學長好嚴苛喔～" },
  { ch=1, img="char5_new/anger" ,text="呆毛小妹妳才是該多動動頭腦思考\n啊！",board_flip="V" },
  { ch=2, img="char2_new/anger" ,text="那就讓你看看我的厲害！" },
  
  { ch=1, text="" }
}

local act5_act3_ = {
  { ch=1, img="char5_new/glad" ,text="喝！電腦少年！\n在下有事前來叨擾！",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" ,text="幹嘛？\n我對鍛鍊肉體可沒有興趣。",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="別誤會，在下這次前來，\n是有些事情想問問你這情報通。" },
  { ch=1, img="char5_new/normal" ,text="關於CuBeat，\n在下相信你應該早收到情報了吧。" },
  { ch=2, img="char3_new/normal" ,text="你這腦袋裝肌肉的老傢伙，\n居然也對CuBeat有興趣？" },
  { ch=2, img="char3_new/glad" ,text="科科，算你沒找錯人。",board_flip="V" },
  { ch=1, img="char5_new/glad" ,text="對未知事物的追求，\n是與生俱來的冒險渴望！" },
  { ch=2, img="char3_new/normal" ,text="那麼，想知道些什麼？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="首先…為什麼相同顏色的方塊排在\n一起就會消掉？" },
  { ch=2, img="char3_new/glad" ,text="好問題。\n事實上這是「集體潛意識」的作用。",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="啥？" },
  { ch=2, img="char3_new/glad" ,text="泛指存在於社會大眾普遍意識深層\n的共同無意識行為。" },
  { ch=2, img="char3_new/glad" ,text="當一樣事情成為人人都知道的通則，\n那麼這件事都會變成真的。" },
  { ch=1, img="char5_new/sad" ,text="可不可以…說得再簡單一點？" ,board_flip="V"},
  { ch=2, img="char3_new/glad" ,text="最初有款方塊遊戲設定了相同顏色\n方塊排在一起就會消掉的規則。" },
  { ch=2, img="char3_new/glad" ,text="當這個設定被大眾所接受後，\n就成了不需解釋的通則，\n簡單來說就是這樣。" },
  { ch=1, img="char5_new/normal" ,text="沒有仔細去想，\n就這麼隨波逐流嗎？",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="在下對這種情況有些難以接受。" },
  { ch=2, img="char3_new/normal" ,text="怎麼樣都好啦，\n話也說夠了，\n你知道接下來我們該做什麼。" ,board_flip="V"},
  
  { ch=1, text="" }
}

local act5_act4_ = {
  { ch=1, img="char5_new/normal" ,text="方塊消失與否的關鍵在顏色啊…" ,board_flip="V",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="那麼，\n該來請教顏色方面的專家了。" },
  { ch=1, img="char5_new/glad" ,text="喝！美術少女！\n在下有事前來叨擾！" },
  { ch=2, img="char4_new/normal" ,text="我們這邊沒有叫外賣喔。",board_flip="HV",effect_a="slide_in"  },
  { ch=1, img="char5_new/sad" ,text="…妳覺得在下是外送拉麵還是外送\n壽司的？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="不不不，在下這次前來，\n是有事情想請教。" ,board_flip="V"},
  { ch=2, img="char4_new/glad" ,text="哎呀，想採訪本小姐嗎？" },
  { ch=2, img="char4_new/normal" ,text="採訪前先約個時間是基本的吧？\n本小姐可不是隨時都有空呢。" },
  { ch=1, img="char5_new/anger" ,text="在下冒然前來是失禮了，\n但這件事非得要請教妳才行！" },
  { ch=1, img="char5_new/anger" ,text="美術少女妳可知道校園內出現了許\n多神秘方塊？" },
  { ch=2, img="char4_new/glad" ,text="不只知道呢，\n本小姐還有能夠操控方塊的能力！" },
  { ch=1, img="char5_new/glad" ,text="果然在下沒找錯人！\n那麼美術少女，\n妳對這神奇現象有什麼頭緒嗎？" },
  { ch=2, img="char4_new/normal" ,text="頭緒？" },
  { ch=2, img="char4_new/glad" ,text="本小姐妙筆生花，\n這不是理所當然的嗎？" },
  { ch=1, img="char5_new/anger" ,text="「理所當然」可不是追求答案該有\n的態度啊。" },
  { ch=2, img="char4_new/anger" ,text="那你就來看看本小姐的勝利是不是\n理所當然！" },
  
  { ch=1, text="" }
}

local act5_act5_ = {
  { ch=1, img="char5_new/anger",text="喝！哈！",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/glad",text="且看在下劈磚絕技！！"},
  { ch=1, img="char5_new/sad",text="啊咧…？",board_flip="V",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=300, y2=400, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250} },
  { ch=1, img="char5_new/sad",text="這次…不算…",effect_w="size_M"},
  { ch=1, img="char5_new/anger",text="是何人干擾在下？\n何來這些方塊？",board_flip="V" },
  { ch=1, img="char5_new/glad",text="正好，\n那就試試在下這招「方塊直擊」！！"},

  { ch=1, text="" }
}

local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="集體潛意識還是集體催眠什麼的…\n問問看魔術專家的意見吧。",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/glad" ,text="喝！魔術小姐！\n在下有事前來叨擾！" },
  { ch=2, img="char6_new/normal" ,text="…嗯。" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="妳可知道最近突然出現的方塊以及\n可以操縱方塊的CuBeat能力？" },
  { ch=2, img="char6_new/anger" ,text="…哼！" },
  { ch=1, img="char5_new/anger" ,text="是殺氣！？\n在下無意間有何冒犯嗎？" },
  { ch=2, img="char6_new/anger" ,text="…少…廢…話！" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=1, img="char6_new/normal" , text="嗯…。\n(差不多也該到尾聲了…)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/glad" , text="呀嘿～這能力真是太有趣了！",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="喂…你！\n(看來下個目標很明顯了！)"},
  { ch=2, img="char1_new/normal" , text="嗯？叫我嗎？"},
  { ch=2, img="char1_new/anger" , text="莫非是…CuBeat同伴的呼喚！？"},
  { ch=1, img="char6_new/normal" , text="…啥？\n(怎麼到處遇到的都是些笨蛋)"},
  { ch=2, img="char1_new/glad" , text="方塊控制能力的名字！\nCube加Beat就是CuBeat！\n我剛取的！"},
  { ch=1, img="char6_new/anger" , text="哼！\n(不知所云！)"},

  { ch=1, text="" }
}

local act6_act2_ = {
  { ch=1, img="char6_new/normal" , text="…。\n(有方塊飛到了教室這邊。)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" , text="啊，妳是魔術社的雪青？\n好酷喔。",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="…嗯。\n(露兩手給她看看吧。)"},
  { ch=2, img="char2_new/glad" , text="耶～！"},
  { ch=1, img="char6_new/normal" , text="…沒什麼。\n(真希望可以不用超能力…) "},
  { ch=2, img="char2_new/normal" , text="啊，對了。\n其實雅季好像也會變魔術呢。"},
  { ch=2, img="char2_new/normal" , text="是方塊魔術喔。\n剛剛才突然學會的。"},
  { ch=1, img="char6_new/anger" , text="哼！\n(讓你輸到以後不敢想方塊！)"},

  { ch=1, text="" }
}

local act6_act3_ = {
  { ch=2, img="char3_new/glad" , text="…CuBeat？\n有趣，控制方塊是吧？",board_flip="H",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="哼！\n(這傢伙看來已經知道些什麼！)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" , text="喔？看來是有個不速之客囉？",board_flip="V"},
  { ch=2, img="char3_new/glad" , text="不，其實是來得正好呢。",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="果然…！\n(這傢伙也要除掉！)"},

  { ch=1, text="" }
}

local act6_act4_ = {
  { ch=1, img="char6_new/normal" , text="…。\n(先檢查一下附近的社團教室。)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char4_new/glad" , text="哎呀，\n今天動起畫筆來特別順手呢。",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="喔？妳是…魔術社的？"},
  { ch=1, img="char6_new/normal" , text="…？\n(她有發覺到什麼嗎？)"},
  { ch=2, img="char4_new/normal" , text="怎麼？\n妳也是是本小姐的粉絲嗎？"},
  { ch=1, img="char6_new/normal" , text="…不。\n(看來只是個普通的笨蛋。)"},
  { ch=2, img="char4_new/glad" , text="本小姐魔幻般的畫技可是不會輸給\n妳的魔術喔。"},
  { ch=1, img="char6_new/anger" , text="…哼！\n(那我倒想見識見識！)"},

  { ch=1, text="" }
}

local act6_act5_ = {
  { ch=2, img="char5_new/normal" , text="今日外頭似乎不大平靜啊。",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char5_new/glad" , text="但身為習武之人，\n心境可不會因此受到影響！"},
  { ch=1, img="char6_new/normal" , text="…喂。\n(試探一下這傢伙)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char5_new/anger" , text="屏除雜念！集中修煉！"},
  { ch=1, img="char6_new/glad" , text="…看！\n(嚇嚇你！)"},
  { ch=2, img="char5_new/normal" , text="喔，有客人啊。"},
  { ch=2, img="char5_new/glad" , text="一定是想來鍛鍊身心對吧？"},
  { ch=1, img="char6_new/normal" , text="…不是。\n(反應意外的平靜？)"},
  { ch=2, img="char5_new/glad" , text="成就不是一蹴可及的，\n實力的背後只有不斷的努力！"},
  { ch=2, img="char5_new/anger" , text="像變魔術那樣無中生有是不可能的！"},
  { ch=1, img="char6_new/anger" , text="哼！\n(就來試試看吧！)"},

  { ch=1, text="" }
}

local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(今天也來練習變魔術吧。)",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="嘿！"},
  { ch=1, img="char6_new/sad" , text="(不行！\n我又不小心用了我的超能力！)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(我真的只想要用手法來變魔術…)",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(但總是不小心施展了超能力…)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(再試一次吧！)",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="唔！\n(好痛！)",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=420, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},effect_w="size_M",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(怎麼又不小心施展超能力了…)"},
  { ch=1, img="char6_new/normal" , text="…咦？\n(這些方塊是？)",board_flip="V"},

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