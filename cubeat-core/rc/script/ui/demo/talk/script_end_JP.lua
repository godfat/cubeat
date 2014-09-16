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
  { ch=1, img="char1_new/normal", text="なるほど、\nこれでキューブを消せるんだ。"},
  { ch=1, img="char1_new/normal", text="知らないうちに、\nすげー力を手に入れたぜ。" },
  { ch=1, img="char1_new/normal", text="いい名前つけなきゃな。\nキューブ…ビート…" },
  { ch=1, img="char1_new/glad", text="そうだ！「キュビート」にしよう！"},
  { ch=1, img="char1_new/glad",text="すげー面白い！" },
  { ch=1, img="char1_new/glad",text="みんなに見せよう！" },
  
  { ch=1, text="" }
}
local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="楽勝楽勝～"},
  { ch=2, img="char2_new/sad", text="うぅ～アキ負けた。", board_flip="HV"},
  { ch=1, img="char1_new/normal", text="でもこうなると、"},
  { ch=1, img="char1_new/normal", text="他にもキュビート能力を\n手に入れたやつがいるんだな。" },
  { ch=1, img="char1_new/glad", text="よぉし！じゃあ、\n最強のキュビート能力者に、\nオレがなってやる！！"}, 
  
  { ch=1, text="" }
}
local act1_act3_ = {
  { ch=1, img="char1_new/glad", text="はッ！面白ぇ！"}, 
  { ch=2, img="char3_new/sad", text="…信じられないです…",board_flip="H", effect_w="size_M" , board="dialogue_bubble_1"}, 
  { ch=1, img="char1_new/glad", text="それじゃ、調べてくれるか？\n他にキュビート能力を\n持ってるやつ。"},
  
  { ch=1, text="" }
}
local act1_act4_ = {
  { ch=1, img="char1_new/glad", text="やっぱキュビート持ってんじゃん！\nでもオレのほうが強い！"},
  { ch=2, img="char4_new/sad", text="ワタクシは負けてないわよ。\nアナタがうるさくて、\n集中できなかっただけよ…",board_flip="HV", pos={x=480, y=230}},
  { ch=1, img="char1_new/normal", text="で、次は誰にするかな…？" },
  { ch=1, img="char1_new/normal", text="なぁ、お前ほかに誰か\nキュビート能力持ってるやつ、\n知ってる？" },
  { ch=2, img="char4_new/anger", text="アナタの質問に答える\n必要はないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}}, 
  
  { ch=1, text="" }
}
local act1_act5_ = {
  { ch=1, img="char1_new/glad", text="よっしゃ！勝ったぜ！" },
  { ch=2, img="char5_new/anger", text="やるじゃないか、\n野球少年。" ,board_flip="HV"},
  { ch=2, img="char5_new/normal", text="でも、私に勝ったくらいじゃ、\n最強になれたわけじゃないぞ。" },
  { ch=1, img="char1_new/anger", text="先輩、\nそれどういう意味だよ？" },
  { ch=2, img="char5_new/anger", text="最強への道の前に\n立ちはだかる最大の敵は、\n他ならぬ…" },
  { ch=2, img="char5_new/glad", text="君自身だ！" },  
  { ch=1, img="char1_new/anger", text="オレ自身！？" },
  { ch=2, img="char5_new/glad", text="そうだ！若いの！\n戦い続けるんだ！" },  
  { ch=2, img="char5_new/glad", text="すべての障壁に挑め！\nすべての相手に挑め！" },  
  { ch=2, img="char5_new/glad", text="自分の限界に挑戦するんだ！\n夕日に向かって走ろう！" },   
  { ch=2, img="char5_new/glad", text="リュタの戦いはこれからだ。" },   
  { ch=1, img="char1_new/glad", text="おおおおおおぉぉ！！", board="dialogue_bubble_2", effect_w="size_L" },
  { ch=2, img="char2_new/glad", text="わぁぁ、リュウタ熱血だね！" },
  { ch=2, img="char4_new/normal", text="何、この少年マンガの\nテンプレエンド。" },  
  { ch=2, img="char6_new/anger", text="いい気にならないで。" },  
  { ch=2, img="char3_new/glad", text="まあ、\n物語は終わりが必要ですからね。" ,board_flip="V"},  
  { ch=1, img="char1_new/glad", text="夕日に向かって走るぜ！", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}
local act1_act6_ = {
  { ch=1, img="char1_new/glad", text="ははッ！勝った勝った！"},
  { ch=2, img="char6_new/sad", text="……",board_flip="H", pos={x=300, y=140}},
  { ch=1, img="char1_new/normal", text="…本当にキューブになったぜ。" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/glad", text="アキが勝ったね。",board_flip="V"},
  { ch=2, img="char1_new/sad", text="こんなのってないよ…" ,board_flip="HV"},
  { ch=1, img="char2_new/normal", text="でもこのパズルゲーム、\nどうしたの？", board_flip="V"},
  { ch=2, img="char1_new/normal", text="これはキュビートってんだ、\n面白ぇだろ。"},
  { ch=1, img="char2_new/normal", text="へぇ～名前もあるんだねぇ。\nアキ初めて聞いたよ。" },
  { ch=2, img="char1_new/glad", text="そりゃ、\nオレがついさっき名付けた\nばっかりだからな！！"},
  { ch=2, img="char1_new/anger", text="キューブがいったい何なのかは、\nさっぱりわからねぇ！"},
  { ch=2, img="char1_new/anger", text="面白ければそれでいいんだ。" },
  { ch=2, img="char1_new/sad", text="ただ、\nまさか最初にお前に負けるとは…"},
  { ch=2, img="char1_new/sad", text="……。"},
  { ch=1, img="char2_new/normal", text="あ、うつモードに入った。" },
  { ch=1, img="char2_new/normal", text="学校内に、\nこのキューブを知ってる人\nいないかなぁ？" },
  
  { ch=1, text="" }
}
local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="あ、もう放課後だね。"},
  
  { ch=1, text="" }
}
local act2_act3_ = {
  { ch=2, img="char3_new/sad", text="これはきっと何がの間違いです…", board="dialogue_bubble_1",board_flip="H"}, 
  { ch=1, img="char2_new/glad", text="イエイ～勝っちゃった～。",board_flip="V"},
  { ch=1, img="char2_new/glad", text="アキ、時間制御以外にも…" },
  { ch=1, img="char2_new/glad", text="もしかしたら、\n「キュビートが上手」って能力が\nあるのかもねぇ！" },
  
  { ch=1, text="" }
}
local act2_act4_ = {
  { ch=1, img="char2_new/glad", text="リアにまで勝っちゃった！\nイエイ～イエイ～！" ,board_flip="V"},
  { ch=2, img="char4_new/anger", text="そんなわけないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  { ch=2, img="char4_new/sad", text="ワ、ワタクシは…\nアナタはワタクシの友たちだから、\nわざと勝たせてあげたのよ！",board_flip="HV",effect_w="size_M" , board="dialogue_bubble_1"},
  { ch=2, img="char4_new/sad", text="ワタクシはもう、\n絵を描く作業に戻るわ。\n邪魔しないで頂戴！"},
  { ch=1, img="char2_new/normal", text="え～。じゃあアキは、\n他に遊んでくれる人\n探しに行こう。"  ,board_flip="V"},
  
  { ch=1, text="" }
}
local act2_act5_ = {
  { ch=1, img="char2_new/glad", text="アキすごいでしょ！\nこれでも鍛える必要あるの？",board_flip="V"},
  { ch=2, img="char5_new/sad", text="…どうやら私のほうが、\n山で野人と\n鍛える必要があるようだ…" ,board_flip="H"},
  
  { ch=1, text="" }
}
local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="イエイ～全勝！" ,board_flip="V"},
  { ch=2, img="char6_new/sad", text="……。" ,board_flip="H"},
  { ch=1, img="char2_new/normal", text="でも、いろんな人に聞いたのに、\n結局誰もキュビートが何なのか、\n知らないね。" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="バイオレットは\n一番わかりそうなのに。" },
  { ch=1, img="char2_new/sad", text="もしかしたら、\nあたしが寝ぼけてるだけ\nかもしれないなぁ？",board_flip="V" },
  { ch=1, img="char2_new/normal", text="あ、そうだ！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="きっとあれだ。\nドラマのベタな展開だね！" },
  { ch=1, img="char2_new/glad", text="あたしきっと、\n夢を見てるんだ！" ,board_flip="V"},
  { ch=2, img="char1_new/glad", text="けっこうおもしろい夢だったね！" ,board_flip="V" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="それでごまかすのは適当すぎるわ。"  ,effect_a="slide_in"}, 
  { ch=2, img="char6_new/sad", text="ふんッ…。",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char5_new/glad", text="夢の中でも鍛えるんだ！",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char3_new/anger", text="夢落ちはタブーですよ！" ,board_flip="V" ,effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="起こさないでよぉ～！" },
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=2, img="char1_new/sad", text="そんな…。",board_flip="HV" },
  { ch=1, img="char3_new/glad", text="次に勝負を挑む時には、\nまず相手を知ることです！"},
  { ch=1, img="char3_new/glad", text="僕は常に\n相手の情報を把握してますけど。\nクク。" },
  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=2, img="char2_new/sad", text="みんなアキをいじめるぅ～。",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="さっき、\nリュウタと言いましたね。\n次のターゲット、決定！"},
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="よし、次はキューブを\nコントロールできる者を\n調べてみましょう…。"},
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=1, img="char3_new/glad", text="僕の推測通り、\nやっぱり君はキューブを\n制御する能力を持ってますね。"},
  { ch=1, img="char3_new/glad", text="ただ、\n技術はまだまだですけど、\nクク。"},
  { ch=2, img="char4_new/sad", text="…こんな賞賛\nわからないわ…。",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="次に行きましょう。"},
  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="鍛錬が必要なのは、\nどっちの頭でしょうね？"},
  { ch=2, img="char5_new/sad", text="う……。" ,board_flip="H"},
  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/glad", text="いきなり仕掛けるとは。\n血の気多すぎますよ、クク。"},
  { ch=2, img="char6_new/sad", text="……。",board_flip="H"},
  { ch=1, img="char3_new/normal", text="でも、\n君は他のキュビート能力者とは\n訳が違いますね。"},
  { ch=1, img="char3_new/normal", text="キュープアクセサリがないのに、\nキュビート能力がある。"},
  { ch=1, img="char3_new/normal", text="これは\n研究のし甲斐がある対象ですね。"},
  { ch=1, img="char3_new/normal", text="それとも、君の体のどこかに\n隠しているんですか？\nクク。",board_flip="V"},
  { ch=2, img="char1_new/glad", text="キュビートに隠された秘密。\nようやくその姿が明かされるのか！"},
  { ch=2, img="char2_new/glad", text="次回、「真相」～。"},
  { ch=1, img="char3_new/anger", text="君たち、いきなり出てきて\n何を言ってるんですか！"},
  { ch=2, img="char5_new/glad", text="鍛錬の道は終わる事なく、\nこのストーリーも同じだ！"},
  { ch=2, img="char4_new/glad", text="何を言ってるのか分らないけど、\n最後だからワタクシも出ないと。"},  
  { ch=2, img="char6_new/sad", text="……。",board_flip="HV"},
  { ch=1, img="char3_new/anger", text="待て、\nどうして僕のエンディングは\n「つづく」なんだ！"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/glad", text="そんなレベルで\nワタクシに挑むなんて、\n侮辱よ。" },
  { ch=2, img="char1_new/sad", text="あんた、\nやっぱキュビート能力者か…\nしかも強ぇ…" ,board_flip="HV"},
  { ch=2, img="char1_new/sad", text="オレに対抗できるのは、\nヒロ先輩だけだと\n思ったんだけどな…" },
  { ch=1, img="char4_new/normal", text="ヒロ先輩？" },
  
  { ch=1, text="" }
}
local act4_act2_ = {
  { ch=1, img="char4_new/glad", text="どう、ワタクシの筆さばきは？" },
  { ch=2, img="char2_new/sad", text="みょん～。" ,board_flip="HV"},
  { ch=1, img="char4_new/normal", text="もう勝手にしなさい。" },
  
  { ch=1, text="" }
}
local act4_act3_ = {
  { ch=1, img="char4_new/glad", text="どう？ワタクシの実力、\n思い知りました？"},
  { ch=2, img="char3_new/sad", text="そんなバカな…",board_flip="H"}, 
  { ch=1, img="char4_new/glad", text="ちゃんとワタクシのことを\nみんなに知らせなさい！"},
  
  { ch=1, text="" }
}
local act4_act4_ = {
  { ch=1, img="char4_new/glad", text="オホホ。ワタクシの筆さばき、\n思った以上に凄いかもしれないわ。"},
  { ch=1, img="char4_new/glad", text="ワタクシが世界を美しくして\n差し上げましょう！" },
  
  { ch=1, text="" }
}
local act4_act5_ = {
  { ch=2, img="char5_new/sad", text="ス、ストレッチを忘れた…" ,board_flip="H"},
  { ch=1, img="char4_new/glad", text="柔道をやめて、\n絵でも描いたら？" },
  { ch=1, img="char4_new/glad", text="でもワタクシは、\n才能のない部員はお断りよ。" },
  { ch=2, img="char1_new/glad", text="オレオレ、入部希望するぜ！" ,board_flip="V"},
  { ch=2, img="char2_new/glad", text="アキも入る！" },
  { ch=2, img="char6_new/normal", text="…今のは？" },
  { ch=2, img="char3_new/normal", text="何ですか、\nこの訳分らない展開は？" },
  { ch=2, img="char5_new/glad", text="待ちたまえ、諸君！\n美術部に入るとは何だね？" },
  { ch=2, img="char5_new/anger", text="今大事なのは、\n我らでキュビートの謎を\n解くことだろう。" },
  { ch=1, img="char4_new/normal", text="だからワタクシは、\nキュビートなんて知らないと\n言っているのよ。" },
  { ch=2, img="char5_new/anger", text="なら、そのキューブを\n操縦している能力は何だね？" },
  { ch=1, img="char4_new/normal", text="別に能力ではないわ。" },
  { ch=1, img="char4_new/glad", text="ワタクシの筆は、この醜い世界を\n美しくすることができる。\nそれだけのことですわ。" },
  
  { ch=1, text="" }
}
local act4_act6_ = {
  { ch=1, img="char4_new/glad", text="あら、\nアナタ本当に運がよろしくってよ！" },
  { ch=1, img="char4_new/glad", text="ワタクシ、ついつい\n予想以上の力を発揮して\nしまったわ。"},
  { ch=2, img="char6_new/sad", text="……。",board_flip="H"},
  { ch=1, img="char4_new/normal", text="アナタと会話するのは難しいわね。"},
  { ch=1, img="char4_new/glad", text="それとも、\nワタクシに感心して、\n言葉も出なくなったの？"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2 , img="char1_new/sad" ,text="さすが伝説のヒロ先輩だぜ…。" ,board_flip="HV"},
  { ch=1 , img="char5_new/glad" ,text="大事なのは自分を鍛える事だ！",board_flip="V" },
  
  { ch=1, text="" }
}
local act5_act2_ = {
  { ch=1 , img="char5_new/glad" ,text="どうだね！",board_flip="V" },
  { ch=2 , img="char2_new/normal" ,text="どうって～？\n面白いでしょう！ " ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="負けても嬉しそうだな？" },
  { ch=2 , img="char2_new/glad" ,text="アキは楽しめればそれでいいの～。" },
  { ch=1 , img="char5_new/glad" ,text="確かに、ある武闘家が言った。\n「戦いの中に答えはある！」" },
  { ch=2 , img="char2_new/normal" ,text="ホントはこのゲームは、\nただのゲームかもしれないよ。\n楽しむ以外の目的は無いとか～" },
  { ch=2 , img="char1_new/glad" ,text="そうだ！\nキュビートは超面白いからな！" },
  { ch=2 , img="char4_new/glad" ,text="次勝負したいなら、\nアポを取りなさい。" },
  { ch=2 , img="char3_new/glad" ,text="まあこんな感じです、クク。",board_flip="V" },
  { ch=2 , img="char6_new/normal" ,text="…あの…私…。" ,board_flip="V"},
  { ch=1 , img="char5_new/glad" ,text="私も挑戦し続けて、\n自分の答えを探すのだ！" },
  
  { ch=1, text="" }
}
local act5_act3_ = {
  { ch=1 , img="char5_new/glad" ,text="ハッ！私の勝利だね！" ,board_flip="V"},
  { ch=2 , img="char3_new/sad" ,text="……。" ,board_flip="H"},
  { ch=1 , img="char5_new/normal" ,text="さっきまで口数が多かったのに。" },
  
  { ch=1, text="" }
}
local act5_act4_ = {
  { ch=1 , img="char5_new/glad" ,text="どうだ！" ,board_flip="V"},
  { ch=2 , img="char4_new/sad" ,text="…二度と\nアナタから出前を頼まないわ…" ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="ここでは\n何も聞き出せそうにないか。\n次は…" },
  
  { ch=1, text="" }
}
local act5_act5_ = {
  { ch=1, img="char5_new/glad" ,text="ふん！瓦割りと変わらんな！",board_flip="V" },
  { ch=1, img="char5_new/normal" ,text="しかしこのキューブは一体？\n調べる必要があるな…" },

  { ch=1, text="" }
}
local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="勝つには勝ったが、\nなぜ勝負を挑まれたのも分からん。\n納得いかないな。",board_flip="V"},
  { ch=2, img="char6_new/sad" ,text="……。",board_flip="H" ,effect_a="slide_in"  },
  { ch=1, img="char5_new/normal" ,text="魔術少女からも、\n何も聞き出せそうにないな。\nただ…" },
  { ch=1, img="char5_new/normal" ,text="キュビートの話になると\n喧嘩を仕掛けてくる。\nまるで何かを隠すようだ。" },
  { ch=1, img="char5_new/glad" ,text="裏がありそうだな。" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=2, img="char1_new/sad" , text="強ぇ…",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="…どう？\n(二度とキューブを\n触りたくないでしょう！)",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="二度と…\nキュビートなんて口にするな！"},
  { ch=2, img="char1_new/anger" , text="え？何で！\nこのゲーム面白いのに！"},
  { ch=1, img="char6_new/normal" , text="…！？\n(なに！？)"},
  { ch=2, img="char2_new/glad" , text="アキもそう思う！面白いよ！",effect_a="slide_in" },
  { ch=2, img="char4_new/sad" , text="ふん！\nワタクシは…別に思わないわ。",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="たまたま、\nワタクシの才能を\n見せられると思って、"},
  { ch=2, img="char4_new/glad" , text="少し遊んでいるだけよ。"},
  { ch=2, img="char5_new/glad" , text="私はいかなる挑戦も拒まない！",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="ふふん、謎は解けました。",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="君は消したいんですね。\n自分からうっかり生まれた産物を！" },
  { ch=2, img="char3_new/glad" , text="まだ間に合うと思いますか？\nクク。"},
  { ch=2, img="char1_new/glad" , text="これからキュビートは、\nヒットゲームになるぜ！",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="…お前ら！\n(こいつら！)"},
  { ch=1, img="char6_new/glad" , text="…！\n(もうどうにでもなれ！)"},

  { ch=1, text="" }
}
local act6_act2_ = {
  { ch=2, img="char2_new/sad" , text="うう、負けた。",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="へへ！\n(また一人やっつけた！)",board_flip="V"},
  
  { ch=1, text="" }
}
local act6_act3_ = {
  { ch=1, img="char6_new/glad" , text="ふん！\n(甘く見ないでね！)",board_flip="V"},
  { ch=2, img="char3_new/sad" , text="そんなバカな…",board_flip="H"},
  { ch=1, img="char6_new/normal" , text="うん…。\n(次は…)"},
  
  { ch=1, text="" }
}
local act6_act4_ = {
  { ch=1, img="char6_new/glad" , text="…！\n(勝ったね！)",board_flip="V"},
  { ch=2, img="char4_new/sad" , text="ワタクシの絵の方がいいのに…",board_flip="HV"},
  { ch=1, img="char6_new/normal" , text="ふん。\n(この子、\nまだ状況が分らないのね)"},
  
  { ch=1, text="" }
}
local act6_act5_ = {
  { ch=1, img="char6_new/glad" , text="はは！\n(どうだ！)",board_flip="V"},
  { ch=2, img="char5_new/sad" , text="まさか私の努力が\nまだ足りないのか！？",board_flip="H"},

  { ch=1, text="" }
}
local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(超能力が暴走して、キューブを\n作っちゃったのかな？)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(なんか学校中に、キューブが\nいっぱい散らかっている…)"},
  { ch=1, img="char6_new/anger" , text="(こうなったら、\nあれをするしか…)"},
  
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