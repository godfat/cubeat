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
  { ch=1, img="char1_new/glad", text="放課後だ放課後！" ,effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="今日はいい天気だぜ！\n熱い練習しようぜ！" },
  { ch=1, img="char1_new/normal", text="ん？ちょっ、何だそれは？"},
  { ch=1, img="char1_new/sad", text="いてててて…" , board="dialogue_bubble_2", pos={x=380, y=180}, effect_w="shake", special={id="cube", x1=500, y1=-50, x2=350, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char1_new/sad", text="何がに当たったような…", effect_w="size_M", board="dialogue_bubble_1" },
  { ch=1, img="char1_new/anger", text="おい！\nなんだこのキューブは！？\n全部打ち返してやるぜ！" },
  { ch=1, text="" }
}

local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="おーい！アキー！\nまだ教室にいるか？",effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="お前もう朝から放課後まで寝たぞ！" },
  { ch=2, img="char2_new/normal", text="……" , board="dialogue_bubble_1" ,board_flip="HV"},
  { ch=1, img="char1_new/glad", text="やっぱりいるか。\n十分に寝ただろ、あのさあのさ…" },
  { ch=2, img="char2_new/normal", text="……" },
  { ch=1, img="char1_new/normal", text="ん？聞こえねぇのか？" },
  { ch=1, img="char1_new/anger", text="おーい！アキ！" ,pos={x=350, y=150}, board="dialogue_bubble_2", effect_w="shake"},
  { ch=2, img="char2_new/normal", text="っあぁ？りゅたか…",effect_a="shake"},
  { ch=1, img="char1_new/normal", text="なにボーっとしてんだ？" , board="dialogue_bubble_1",effect_w="size_M" },
  { ch=2, img="char2_new/normal", text="なんだかあたまがくらくらする…"},
  { ch=2, img="char2_new/sad", text="なにかにあたまが当たった感じ"},
  { ch=2, img="char2_new/sad", text="あたまの中にきゅーぷがいっぱい" },
  { ch=2, img="char2_new/anger", text="それにアキが強くイメージすると、\nきゅーぷたちは\n思う通りに消えるんだ"},
  { ch=1, img="char1_new/anger", text="おいおい…\nまさかお前もキュビート能力が\n手に入れたか？"},
  { ch=2, img="char2_new/normal", text="キュゥビえ？なにそれ？"},
  { ch=1, img="char1_new/anger", text="アキッ！勝負だぜ！" , board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=2, img="char2_new/normal", text="ええぇ！？" },
  { ch=1, img="char1_new/anger", text="誰が一番キュビートを\nうまく使えるか\nはっきりしようぜ", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}

local act1_act3_ = {
  { ch=1, img="char1_new/normal", text="お~コンピュータ部の設備って\nすごそうに見えるぜ",effect_a="slide_in" },
  { ch=2, img="char3_new/normal", text="何の用ですか？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="あ、いや別に、\nただ一つ聞きたい…" },
  { ch=1, img="char1_new/normal", text="知ってる？\n学校で大量なキューブが出現したぜ" },
  { ch=2, img="char3_new/normal", text="ふんっ、\nボクの情報収集能力を\n挑戦したいですか？"},
  { ch=1, img="char1_new/normal", text="それじゃ知ってるんだな、\nキューブを操れる能力を\n持つ人がいるぜ"},
  { ch=1, img="char1_new/glad", text="その能力は\n「キュビート」というんだぜ！\nオレが名づけだ"},
  { ch=2, img="char3_new/glad", text="「キュビート」ですか？",board_flip="V"},
  { ch=2, img="char3_new/glad", text="キミは\n他に誰かいると聞きたいだね、"},
  { ch=2, img="char3_new/glad", text="そのキュビートっていう\n能力を持つ人？"},
  { ch=2, img="char3_new/glad", text="フフフ…クククク…"},
  { ch=2, img="char3_new/anger", text="ボクも\n持ってるんですよォ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act1_act4_ = {
  { ch=1, img="char1_new/normal", text="美術室…これは壮観だぜ",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="ジークがここにキュビートを\nもってる人がいるって。\nそれは本当かな？" },
  { ch=2, img="char4_new/normal", text="そこ、何をしてる！\n絵を描くのを邪魔しないて頂戴！",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="絵？\nこれ全部お前が描いたのか？" }, 
  { ch=1, img="char1_new/anger", text="きれい！\n本当すげーな！"},
  { ch=2, img="char4_new/glad", text="ふん！こ、こんな程度の賞賛で\nワタクシが喜ぶと思って？"}, 
  { ch=1, img="char1_new/glad", text="っでお前もキュビート能力\n持ってるでしょ！"},
  { ch=1, img="char1_new/glad", text="キューブを操れる\nキューブを並んで消すあの能力！"},
  { ch=2, img="char4_new/normal", text="あなたが何を言ってるのが\nわからないわ"},
  { ch=1, img="char1_new/sad", text="え？\nまさかジーク教えたのは偽情報？"},
  { ch=1, img="char1_new/sad", text="がっかりだぜ…"},
  { ch=2, img="char4_new/anger", text="無礼な！誰もワタクシ\nに失望できないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  
  { ch=1, text="" }
}

local act1_act5_ = {
  { ch=1, img="char1_new/normal", text="翔鯨道場…\nいつの間にかここに来ちまったぜ",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="伝説のヒロ先輩いるかな？" },
  { ch=1, img="char1_new/glad", text="もし本当に伝説通りの強さなら、\n彼に勝ったオレは最強だぜ！"},
  { ch=2, img="char5_new/normal", text="ははは、野球くん！\n君も心身鍛錬しにきたんかね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="心身鍛錬？"},
  { ch=1, img="char1_new/normal", text="違うぜ、オレはオレと同じ\nキューブを消せる能力\nを持ってる人を探してるだぜ" },
  { ch=2, img="char5_new/glad", text="ははッ！\nそれならここで間違ってないね、\n野球くん！"},
  { ch=1, img="char1_new/anger", text="やっぱり！" },
  { ch=1, img="char1_new/anger", text="センパイ！\nオレと勝負しようぜ！" },
  { ch=1, img="char1_new/anger", text="お前に勝てば\nオレは最強になれるぜ！" },
  { ch=1, img="char1_new/normal", text="…そんな気がする"},
  { ch=2, img="char5_new/normal", text="最強になりたいかね？"},
  { ch=2, img="char5_new/anger", text="野球くん、\n君は考えたことあるかね？\n最強とは何だっと？"},
  { ch=2, img="char5_new/normal", text="最強になったら、\nその次に何がある？"},
  { ch=1, img="char1_new/anger", text="そりゃもちろん…", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=360, y=100} },
  { ch=1, img="char1_new/anger", text="そりゃ…" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char1_new/normal", text="そうだな、考えたことないぜ"},
  { ch=2, img="char5_new/anger", text="まず頭を使うんだ！\n若いの！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=380, y=80} },
  { ch=1, img="char1_new/sad", text="説教はオレに勝ってからしようぜ…"},
  { ch=2, img="char5_new/glad", text="なら拙者も手加減なしで行くぞ" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, text="" }
}

local act1_act6_ = {
  { ch=1, img="char1_new/normal", text="キュビートって不思議の力、\n言ってみれば\n魔術みたいなもんだぜ",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="魔術部の人\n何が知ってるかもしれないぜ"},
  { ch=1, img="char1_new/glad", text="ヤッハロー、\n魔術部のみんなこんにちは！" },
  { ch=2, img="char6_new/normal", text="……" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="あの…こんにちは？"},
  { ch=2, img="char6_new/glad", text="……"},
  { ch=1, img="char1_new/glad", text="おお！すげぇ！さすが魔術師、\n面白いあいさつだぜ"},
  { ch=2, img="char6_new/normal", text="……うん"},
  { ch=1, img="char1_new/normal", text="お前本当魔術が得意だな" },
  { ch=1, img="char1_new/normal", text="じゃあ、\nキューブに関するある「魔術」も、\nお前は問題なさそうだぜ？"},
  { ch=2, img="char6_new/anger", text="ふんっ！！"},
  { ch=1, img="char1_new/anger", text="ええ？いきなり宣戦かよ！？" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/normal", text="あ、リュタだ～",effect_a="slide_in" },
  { ch=2, img="char1_new/glad", text="アキ、ちょうどいい、\n面白れもん見せてあげるぜ"  ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="わい～本当ぉ？何がな\nウサギみたいなねこかな？" ,board_flip="V"},
  { ch=2, img="char1_new/normal", text="ぜってぇ違う。\nそれ面白いのか？"},
  { ch=1, img="char2_new/anger", text="あったら面白いのにぃ"},
  { ch=2, img="char1_new/anger", text="犬とか猫とか置いといて！\nこれ試しようぜ！"},
  
  { ch=1, text="" }
}

local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="今日はいい天気だねぇ～"},
  { ch=1, img="char2_new/normal", text="なんか外の雲をずぅーと見てて\n一日過ごせる感じ" },
  { ch=1, img="char2_new/normal", text="これですっごく幸せだよね" },
  { ch=1, img="char2_new/sad", text="いたッ！" ,effect_w="shake", board="dialogue_bubble_2", pos={x=370, y=120}, special={id="cube", x1=500, y1=-50, x2=300, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char2_new/anger", text="もー！誰だよアキの頭をぶったの！", effect_w="size_M", board="dialogue_bubble_1" ,board_flip="V" },
  { ch=1, img="char2_new/normal", text="え？誰もいないよ？",board_flip="V"},
  { ch=1, img="char2_new/normal", text="変だよねぇ…\nでも何が目の前に\nキューブがいっぱいだねぇ…" },
   
  { ch=1, text="" }
}

local act2_act3_ = {
  { ch=1, img="char2_new/normal", text="ここにコンピュターがいっぱだね\nたぶんコンピュータ部かな" ,effect_a="slide_in" },
  { ch=1, img="char2_new/normal", text="パソコンでテレビと似てるねぇ。\nでもパソコンより、\nアキはテレビのほうが好き"},
  { ch=1, img="char2_new/normal", text="だってテレビのほうが\nあたま使わなくっていいねぇ"},
  { ch=2, img="char3_new/normal", text="お…ちょうどよかった、\n直接あらわれたですね",board_flip="HV" ,effect_a="slide_in"},  
  { ch=2, img="char3_new/glad", text="先の対戦、見ましたよ、\nクク",board_flip="V"},
  { ch=2, img="char3_new/glad", text="キミのカウントダウン時間が\n長いとは、"},
  { ch=2, img="char3_new/glad", text="どうやらキミは時間を\n制御できる特殊属性ですね" },
  { ch=1, img="char2_new/normal", text="へ？なに？時間を制御？\nアキが時間をできるというの？"},
  { ch=1, img="char2_new/normal", text="そういえば、\nアキはいつも外をぼーっと見てると\n時間がゆっくりになったと思うねぇ"},
  { ch=1, img="char2_new/anger", text="そしてつい寝ちゃうと、\n起きたらもう夜だよぉ、\n逆に時間がすっごくはやいと思った" ,board_flip="V"},
  { ch=1, img="char2_new/glad", text="それがあなたの言う時間制御？\n特殊能力だったんだ、\n知らなかったねぇ"},
  { ch=2, img="char3_new/normal", text="えッいや、\nそれはたぶん違うと思います…",board_flip="V"},  
  { ch=1, img="char2_new/anger", text="なによ、あなた先アキに\n時間制御能力があると\n言ったのにぃ"},
  { ch=2, img="char3_new/anger", text="話が通じない！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act2_act4_ = {
  { ch=1, img="char2_new/normal", text="わぁ～\nリアの絵相変わらずきれいだねぇ" ,effect_a="slide_in" },
  { ch=2, img="char4_new/glad", text="ん？アキか",board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="アナタが本当に絵の良さがわかる\n品性があるかどうか分らないけれど"},
  { ch=2, img="char4_new/glad", text="ワタクシの絵は誰にも\n失望させたことないわ"},
  { ch=1, img="char2_new/glad", text="へぇ～リアすごぉい～",board_flip="V"},
  { ch=2, img="char4_new/normal", text="それで、どうなさったの、\nわざわざ部室まで会いにきて？"},
  { ch=1, img="char2_new/normal", text="それがね、何が\nアキはすごくなった感じがするねぇ、\n特殊能力あるよ！" ,board_flip="V"},
  { ch=2, img="char4_new/glad", text="寝ても覚めても夢を見てる以外に\nほかに何の特殊能力があって？"},
  { ch=1, img="char2_new/glad", text="え、そんなのあるの？\nアキいきなり新しい特殊能力\n増えちゃったぁ！" ,board_flip="V"},
  { ch=2, img="char4_new/normal", text="アナタがいつも白昼夢を見てる\nという意味よ"},
  { ch=1, img="char2_new/anger", text="そんなことないよぉ、\nアキの能力すごいだよ！\nほら見て！" },
  
  { ch=1, text="" }
}

local act2_act5_ = {
  { ch=1, img="char2_new/normal", text="柔道部って木多いねぇ、\n森みたい",effect_a="slide_in" },
  { ch=2, img="char5_new/glad", text="ハハハッ！アホ毛くん！\n君も心身鍛錬しに来たのかね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="うわっ！\nジャングルの野人があらわれた！",board_flip="V"},
  { ch=2, img="char5_new/sad", text="ジャングルの…野人…",board_flip="V"},
  { ch=2, img="char5_new/normal", text="いいか、\n拙者は翔鯨道場師範、\nジャングルの野人なんかじゃない" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="えぇ～つまんない～\n野人があらわれたほうが\n面白いのにぃ",},
  { ch=2, img="char5_new/anger", text="野人とかもうどうでもいい！"},
  { ch=2, img="char5_new/glad", text="君がここに来た目的は？\nキッチリ鍛えたいがね？"},
  { ch=1, img="char2_new/normal", text="鍛えとか疲れそうだねぇ" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="アキ疲れるのヤ～" ,board_flip="V"},
  { ch=2, img="char5_new/anger", text="若者よ、\nそんな体たらくでどうする！\n戦うを覚悟するんだ！"},
  
  { ch=1, text="" }
}

local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="魔術部だァ！\nアキも魔術だいすき！" ,board_flip="V",effect_a="slide_in" },
  { ch=2, img="char6_new/normal", text="……" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/normal", text="あ、\nあなたが魔術部のバイオレットだね" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="魔術がすごく上手と聞いたよ、\nアキにも見せてくれる？"},
  { ch=2, img="char6_new/glad", text="……うん"},
  { ch=1, img="char2_new/glad", text="イエイ～ハトだ！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="実はアキねこのほうが好きなんだ",board_flip="V"},
  { ch=2, img="char6_new/normal", text="……おう"},
  { ch=1, img="char2_new/normal", text="そうだ、バイオレット、\nキューブを変化するキュビートも\n魔術の一種なの？"},
  { ch=2, img="char6_new/normal", text="……違う"},
  { ch=1, img="char2_new/anger", text="ああ～残念。\nわたしも魔術できると思ったんだァ",board_flip="V"},
  { ch=2, img="char6_new/anger", text="そんな簡単…じゃない！"},
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=1, img="char3_new/normal", text="ん？",effect_a="slide_in" },
  { ch=2, img="char1_new/glad", text="あはっ！\n最強のキュビート能力者オレはなる！" ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="おい、そこの赤毛" },
  { ch=2, img="char1_new/normal", text="よう、何の用だぜ？"},
  { ch=1, img="char3_new/glad", text="そのキューブ型リストバンド…\n珍しいですね、クク"},
  { ch=2, img="char1_new/glad", text="かっこいいだろ！\nでもキューブと関わるなら\nもっといい物あるぜ…"},
  { ch=1, img="char3_new/normal", text="キューブの制御能力ですね"},
  { ch=2, img="char1_new/anger", text="キュビートだぜ！、オレが名付だ！"},
  { ch=2, img="char1_new/normal", text="でもなぜ分る？"}, 
  { ch=1, img="char3_new/glad", text="いい名前ですね\nなぜ分るは重要じゃない、\n重要なのは…"},
  { ch=2, img="char1_new/glad", text="お前もキュビートできるだろ！\nじゃ勝負るぜ！"},
  { ch=1, img="char3_new/anger", text="望むところです！", effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=1, img="char3_new/normal", text="情報によると\nあの能天気なアキも\nキューブを制御出来るだそうです",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="どうやら頭の良さは\nキューブを制御できる\n条件じゃなさそうですね"},
  { ch=1, img="char3_new/normal", text="おい、アキ。\nちょっと聞きたいことがあります"},
  { ch=2, img="char2_new/normal", text="そぉう？\nでもアキ今頭がクラクラするよぉ",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="キューブが頭に当たったん？\n大丈夫です、\n頭が悪くなる心配はないです"},
  { ch=1, img="char3_new/normal", text="だってこれ以上\nバカになるのは不可能です"},
  { ch=2, img="char2_new/anger", text="え～ひどい！"},
  { ch=2, img="char2_new/normal", text="アキ今特殊な能力が使えるよ！\nキューブを操れるよ！"},
  { ch=1, img="char3_new/glad", text="クク、やっぱり！\nキミもキューブを制御する能力が\nあります"},
  { ch=2, img="char2_new/anger", text="まさかリュタと同じ、\nまたアキと対戦したいなの？"},
  { ch=2, img="char2_new/glad", text="でもそれって\nアキはモテモテみたいだね！"},
  { ch=1, img="char3_new/normal", text="そうですね"},
  { ch=1, img="char3_new/glad", text="いいカモですよ"},
  
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="今日も何が起こってますか？\nククク"},
  { ch=1, img="char3_new/glad", text="僕が作った校内情報交換システムは\nいつも面白い情報を\n持ってくれますね"},
  { ch=1, img="char3_new/glad", text="どれどれ…"},
  { ch=1, img="char3_new/glad", text="「伝説によるとは学校マスコット\n飛天クジラの大好物は\nポテチなんだ！」"},
  { ch=1, img="char3_new/glad", text="「実はこの学校の校長は\n宇宙リトルブルーマン！」"},
  { ch=1, img="char3_new/anger", text="なぜこんな\nわけ分らない情報\nばっかですか！" ,effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=360, y=260}},
  { ch=1, img="char3_new/normal", text="うん？これは何ですか？", board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char3_new/normal", text="「学校内に大量な\nミステリキューブが発生、\nこのキューブの源も\n用途はいまだ不明…"},
  { ch=1, img="char3_new/normal", text="…目撃者によると、\n一部の人間がキューブを使って\n対戦する能力を持ってると…」"},  
  { ch=1, img="char3_new/glad", text="面白い、キューブを操れるですね？"},
  { ch=1, img="char3_new/glad", text="ちょうどいい、\nたしかにいくつ同じようなキューブ\nを見たんです、\n早速試してみましょう、クク"},
  
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=2, img="char4_new/normal", text="うん？\nコンピューター部の、何の用？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="そうですね、ただここがちょっと\n尋常じゃないと思っただけです、\nクク",effect_a="slide_in"},
  { ch=2, img="char4_new/glad", text="あら？\n今更ワタクシの尋常じゃない技法に\n気付いたの？"},
  { ch=1, img="char3_new/normal", text="いや。\nそういうこと言ってるじゃないです"},
  { ch=2, img="char4_new/glad", text="ふふん、\nたしかにワタクシの絵も\n尋常な価格で買えるものじゃないわ"},
  { ch=1, img="char3_new/normal", text="話を聞いてください、おい"},
  { ch=2, img="char4_new/glad", text="尋常な賞賛では\nワタクシは受付けないわ"},
  { ch=1, img="char3_new/anger", text="話が通じない！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=360, y=260}},

  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="そろそろキュビート能力の\n獲得条件を見えてきました",effect_a="slide_in" },
  { ch=1, img="char3_new/normal", text="リアとアキの髪飾り、\nリュタのリストバンド…" },
  { ch=1, img="char3_new/normal", text="キューブ型アクセサリを着けて、\nキューブに触ると…" },
  { ch=1, img="char3_new/glad", text="キュビート能力を手に入れる！"},
  { ch=1, img="char3_new/glad", text="次に条件を満たす可能のは\nこのベルトに\nキューブがついてる奴です" },
  { ch=2, img="char5_new/normal", text="おう？パソコン少年、\n君も心身鍛錬したいがね？",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="興味ありません"},
  { ch=2, img="char5_new/glad", text="遠慮するな！\nずっとパソコンいじるようで、\nきっと運動不足だろ！"},
  { ch=1, img="char3_new/normal", text="脳筋のひとに\n説教されたくありません" },
  { ch=2, img="char5_new/glad", text="パソコンより、\n大事なのは君の頭だ！\nその頭と直結する体だ！"},
  { ch=1, img="char3_new/anger", text="うるさい！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/normal", text="最新の情報では魔術部も\nキュビートが上手な人がいると",effect_a="slide_in" },
  { ch=1, img="char3_new/normal", text="確かに魔術部に\nマジックがうまいが\n口ベタな女マジシャンがいますね…"},
  { ch=2, img="char6_new/glad", text="……はい",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="いいですな、\nどうやら僕が探している人は君ね"},
  { ch=2, img="char6_new/normal", text="……？"},
  { ch=1, img="char3_new/normal", text="待て、\n君はキューブ型のアクセサリが\n付けてませんね"},
  { ch=1, img="char3_new/normal", text="キュビート能力者\nではありませんか…？"},
  { ch=2, img="char6_new/anger", text="ふんッ！"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/normal", text="あら？",effect_a="slide_in"},
  { ch=2, img="char1_new/glad", text="見つけたぞ～\nあんたを探していたんだぜ！",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="ほう、どうやらワタクシの名は\n確実に広めているわ！"},
  { ch=2, img="char1_new/anger", text="オレはキュビート能力者を\n探しているぜ！"},
  { ch=2, img="char1_new/glad", text="お前も持ってると聞いて、\n早速対戦しようぜ！"},
  { ch=1, img="char4_new/normal", text="キュビート？何ですそれ？\nワタクシ知らないわ"},
  { ch=2, img="char1_new/normal", text="そっか、\nだってさき名付だばかりだぜ"},
  { ch=1, img="char4_new/normal", text="なんが適当ですわね、\n興味ありませんわ"},
  { ch=2, img="char1_new/anger", text="じゃ適当にオレと勝負しようぜ！"},
  
  { ch=1, text="" }
}

local act4_act2_ = {
  { ch=1, img="char4_new/normal", text="まだ教室にいますの、アキ？\nもう放課後ですのよ",effect_a="slide_in"},
  { ch=2, img="char2_new/normal", text="あ…",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/normal", text="ボーっとしない、\nワタクシが\n新に目覚めた能力を見よう！"},
  { ch=2, img="char2_new/glad", text="みょん？\nなんかかわいいね！"},
  { ch=1, img="char4_new/normal", text="みょんじゃなくて、見ようですわ"},
  { ch=2, img="char2_new/glad", text="みょん～みょん～\nリアかわいい～"}  ,
  { ch=1, img="char4_new/anger", text="いい加減、まじめにやりなさい！"} ,

  { ch=1, text="" }
}


local act4_act3_ = {
  { ch=1, img="char4_new/normal", text="コンピュター部は\n学校情報ネットが\nあるだそうですわ",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="もっとワタクシの存在を\n大衆に報せる時が来たわ"},
  { ch=2, img="char3_new/normal", text="何の用ですか？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="ワタクシが訪問するなんて、\n貴方達運がいいですわ"},
  { ch=2, img="char3_new/normal", text="あ、そう"},  
  { ch=1, img="char4_new/normal", text="して貴方達のヘヤで\n全然美しくないわ"},
  { ch=1, img="char4_new/normal", text="ワタクシの絵を掛けで\nあげましょうか？"},
  { ch=1, img="char4_new/glad", text="勿論ただではありませんけど、\nホホ"},
  { ch=2, img="char3_new/normal", text="って一体何の用ですか？"},
  { ch=1, img="char4_new/normal", text="ワタクシのことを知らないなんて"},
  { ch=2, img="char3_new/glad", text="無駄の情報は集めないです",board_flip="V" },
  { ch=1, img="char4_new/anger", text="無礼な！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=300, y=100}},
  
  { ch=1, text="" }
}

local act4_act4_ = {
  { ch=1, img="char4_new/normal", text="よし、これで絵が完成ですわ",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="まさに完璧ね、ワタクシの絵"},
  { ch=1, img="char4_new/sad", text="ヒイ！",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=170, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},board="dialogue_bubble_2",board_flip="V", pos={x=300, y=100} },
  { ch=1, img="char4_new/anger", text="何なのこのキューブは？",effect_w="size_M",board="dialogue_bubble_1",board_flip="V"},

  { ch=1, text="" }
}

local act4_act5_ = {
  { ch=1, img="char4_new/glad", text="喜びなさい！\n柔道部部員よ！\nワタクシが参りましたわよ！" ,effect_a="slide_in"},
  { ch=2, img="char5_new/normal", text="お？美術少女、\n君も心身鍛錬しに来たのがね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char5_new/anger", text="だが残念！\n拙者は今ある難題を探求して\n手が離せないんだね"},
  { ch=1, img="char4_new/anger", text="ワタクシが自ら訪問したのは\nどんなに光栄が分って？"},
  { ch=1, img="char4_new/anger", text="誰もワタクシを拒絶出来ないわ！"},
  { ch=2, img="char5_new/normal", text="こりゃ失礼"},
  { ch=2, img="char5_new/anger", text="君は学校内に突然現れた\nミステリキューブを知ってるがね？"},
  { ch=2, img="char5_new/anger", text="拙者は今その原因を追っているだね"},
  { ch=2, img="char5_new/normal", text="ふむ…拙者の予感だが…"},
  { ch=2, img="char5_new/glad", text="こんな時に尋ねた君も、\n例の能力があるではないか" },
  { ch=1, img="char4_new/glad", text="あら、ストレートですわね、\n後悔しないわよ"},
  
  { ch=1, text="" }
}

local act4_act6_ = {
  { ch=1, img="char4_new/normal", text="うむ、魔術部か",effect_a="slide_in" },
  { ch=2, img="char6_new/glad", text="ハイ…",board_flip="HV"},
  { ch=1, img="char4_new/normal", text="わざわざ迎えにこなくてもいいよ、\nちょっと通っただけよ"},
  { ch=2, img="char6_new/normal", text="……"},
  { ch=1, img="char4_new/glad", text="まあ、\n貴方運がいいわね、\n今日はいい気分だわ"},
  { ch=1, img="char4_new/glad", text="ついでにワタクシの\n画筆のすごさを\nお見せしましょう！"},
  { ch=2, img="char6_new/anger", text="ふんッ！"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2, img="char1_new/glad" ,text="ハイ～みんな～\nキュビートやらない？ " ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char5_new/normal" ,text="キュピット？\nヘイ！野球くん！\nそのキュピットは？ ",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/normal" ,text="超面白くて不思議な力だぜ！" },
  { ch=2, img="char1_new/glad" ,text="学校内に発生したキュープを\n消せるんだぜ！" },
  { ch=1, img="char5_new/glad" ,text="どうやら野球くんも\n同じ目に遭ったか…" },
  { ch=2, img="char1_new/anger" ,text="え！\nお前は伝説のヒロ先輩じゃねぇ？" },
  { ch=2, img="char1_new/glad" ,text="先輩もキュビート能力持ってる？\n一戦やらないか？" },
  { ch=1, img="char5_new/normal" ,text="その前に聞きたいが\n野球くんはどうやって\n能力を手に入った？" },
  { ch=2, img="char1_new/glad" ,text="突然使えたぜ！\nスゲーだろ！" },
  { ch=1, img="char5_new/normal" ,text="まさか野球くん\n君は能力について\nおかしいと思わないかね？" },
  { ch=2, img="char1_new/normal" ,text="ないだぜ" },
  { ch=1, img="char5_new/anger" ,text="頭を使えたまえ、若いの！" },
  { ch=1, img="char5_new/glad" ,text="力の原因を考えよ！\n力の持つ者の責任を考えよ！" },
  { ch=2, img="char1_new/anger" ,text="あ、分かった！" },
  { ch=2, img="char1_new/glad" ,text="たぶんオレは天才だからだぜ！" },
  { ch=1, img="char5_new/normal" ,text="野球くん…\n自信があるのは悪じゃない"},
  { ch=1, img="char5_new/anger" ,text="だが\n力と相応の謙虚を覚えるがいい！" },
  
  { ch=1, text="" }
}

local act5_act2_ = {
  { ch=1, img="char5_new/normal" ,text="手掛かりはあるが、\n謎はまだ解けないな" ,board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" ,text="あ、こんにちは～\nキュビートやらない？" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="拙者の前に出た人は\n全部キュビート能力者とはね、\nもう驚かないかもね" },
  { ch=1, img="char5_new/glad" ,text="ハッ！\nその前にアホ毛ちゃんに尋ねたい！" },
  { ch=1, img="char5_new/anger" ,text="君はみんなの目の前に\nキュビート対戦をする\n目的は何だね？" },
  { ch=2, img="char2_new/normal" ,text="うん～\nあたし思ったことないね～\nうんとね～" },
  { ch=2, img="char2_new/glad" ,text="7回勝利すると神様を召喚できる、\nそれで願いを一つ叶えるかな？" },
  { ch=1, img="char5_new/anger" ,text="そのゲームはそんな要素はない！\nドラゴン○じゃないね！" },
  { ch=2, img="char2_new/sad" ,text="じゃこのゲームをやった人は\n勝ち続かないと死ぬとか" },
  { ch=1, img="char5_new/sad" ,text="恐ろしすぎるわ！\nなんかの呪われたゲームか！？" ,board_flip="V"},
  { ch=2, img="char2_new/anger" ,text="ヒロ先輩細かすぎる～" },
  { ch=1, img="char5_new/anger" ,text="アホ毛ちゃんは\n少し頭使うべきだね！",board_flip="V" },
  { ch=2, img="char2_new/anger" ,text="じゃあたしすごさを見せてあげる！" },
  
  { ch=1, text="" }
}

local act5_act3_ = {
  { ch=1, img="char5_new/glad" ,text="ハッ！パソコン少年！\n少し訪ねたい事があるだがね！",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" ,text="何ですか？\n身体を鍛える興味はないです",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="誤解するな、\n今回来たのは、\n情報通の君に聞きたい事があるんだ" },
  { ch=1, img="char5_new/normal" ,text="キュビートの件、\n君もその情報を掴んだのだね" },
  { ch=2, img="char3_new/normal" ,text="脳筋で時代遅れの貴方も\nキュビートに興味があるですか？" },
  { ch=2, img="char3_new/glad" ,text="クク、その通りです",board_flip="V" },
  { ch=1, img="char5_new/glad" ,text="未知への探求も\n人が生れ付き冒険への欲望だ！" },
  { ch=2, img="char3_new/normal" ,text="それで、何が知りたいですか？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="まずは…なぜ同じ色のキュープが\n並んだら消えるがね？" },
  { ch=2, img="char3_new/glad" ,text="いい質問です\n実はそれは「普遍的無意識」が\n原因です",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="何だねそれは？" },
  { ch=2, img="char3_new/glad" ,text="簡単に言うと全人間の\n無意識の深層に存在する\n共通の考えです" },
  { ch=2, img="char3_new/glad" ,text="あるルールが\n誰もが知ってる事になったら\nそれが常識に変える" },
  { ch=1, img="char5_new/sad" ,text="えッ…もう少し簡単に言うと？" ,board_flip="V"},
  { ch=2, img="char3_new/glad" ,text="最初はあるパズルゲームが\n同じ色のブロックが並んだら\n消えるルールがあります" },
  { ch=2, img="char3_new/glad" ,text="その設定をみんなが受け入れた、\nそして常識になった、\nそんな感じですね" },
  { ch=1, img="char5_new/normal" ,text="よく考えもせずに、\nそのまま流されるのか？",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="拙者は少し納得いかんだね" },
  { ch=2, img="char3_new/normal" ,text="まあどうでもいいです、\nお喋りはここまで\nやる事は分りますね" ,board_flip="V"},
  
  { ch=1, text="" }
}

local act5_act4_ = {
  { ch=1, img="char5_new/normal" ,text="キュープを消せる要は色か…" ,board_flip="V",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="それなら、\n色の専門家に聞くのが一番だね" },
  { ch=1, img="char5_new/glad" ,text="ハッ！美術少女！\n少し訪ねたい事があるだがね！" },
  { ch=2, img="char4_new/normal" ,text="出前なんで頼んでないわよ",board_flip="HV",effect_a="slide_in"  },
  { ch=1, img="char5_new/sad" ,text="…拙者はラーメン屋か寿司屋かね？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="いやいや、拙者が来たのは、\n訪ねたい事があるのだね" ,board_flip="V"},
  { ch=2, img="char4_new/glad" ,text="あら、\nワタクシを取材したいですか？" },
  { ch=2, img="char4_new/normal" ,text="取材ならアポを取るのが基本よ？\nワタクシも暇じゃなくってよ" },
  { ch=1, img="char5_new/anger" ,text="突然来たのは侘びよう、\nだがこの件君の力が必要だね！" },
  { ch=1, img="char5_new/anger" ,text="美術少女君は\n学園内に大量なキュープが\n発生したのは知ってるがね？" },
  { ch=2, img="char4_new/glad" ,text="知ってるも何も、\nワタクシはキュープの制御も\nできるわよ！" },
  { ch=1, img="char5_new/glad" ,text="やはりそうか！なら美術少女よ\n君はこの現象どう思うがね？" },
  { ch=2, img="char4_new/normal" ,text="どう思う？" },
  { ch=2, img="char4_new/glad" ,text="ワタクシの画筆は世界を変える、\n当り前ですわよ？" },
  { ch=1, img="char5_new/anger" ,text="「当り前」\nは探求する時の態度じゃないだね" },
  { ch=2, img="char4_new/anger" ,text="ならワタクシの勝利は\n当り前どうか確かめよ！" },
  
  { ch=1, text="" }
}

local act5_act5_ = {
  { ch=1, img="char5_new/anger",text="喝！破ァ！",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/glad",text="わが技を見よ！！"},
  { ch=1, img="char5_new/sad",text="なん…！",board_flip="V",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=300, y2=400, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250} },
  { ch=1, img="char5_new/sad",text="これは…ノーカン…",effect_w="size_M"},
  { ch=1, img="char5_new/anger",text="拙者を邪魔したのは誰じゃ？\n何だこのキュープの山？",board_flip="V" },
  { ch=1, img="char5_new/glad",text="よかろう、\n拙者の技「方塊直撃」を\n喰らうがいい！！"},

  { ch=1, text="" }
}

local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="普遍的無意識か何とか…\nここは魔術の専門家の意見を聞こう",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/glad" ,text="ハッ！魔術ちゃん！\n少し訪ねたい事があるだがね！" },
  { ch=2, img="char6_new/normal" ,text="…うん" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="君は最近突然発生したキュープと\nそれを制御するキュビート能力を\n知ってるがね？" },
  { ch=2, img="char6_new/anger" ,text="…ふん！" },
  { ch=1, img="char5_new/anger" ,text="殺気！？\n拙者が何が気分を害したかね？" },
  { ch=2, img="char6_new/anger" ,text="…だ…ま…れ…！" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=1, img="char6_new/normal" , text="うん…\n(大体片付けたかな…)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/glad" , text="ヒャッハーこの能力面白れぇ！",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="おい…お前！\n(次の目標が決めたね！)"},
  { ch=2, img="char1_new/normal" , text="ん？俺か？"},
  { ch=2, img="char1_new/anger" , text="まさか…\nキュビート能力者同士の呼掛けか？"},
  { ch=1, img="char6_new/normal" , text="…は？\n(どいつもこいつもバカばっか)"},
  { ch=2, img="char1_new/glad" , text="キュープを制御する能力の名前！\nキュープとビットを掛けで\nキュビート！俺が先名づけた！"},
  { ch=1, img="char6_new/anger" , text="ふん！\n(訳が分らないよ！)"},

  { ch=1, text="" }
}

local act6_act2_ = {
  { ch=1, img="char6_new/normal" , text="…\n(教室の方にも\nキュープが飛んできたね)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" , text="あ、\n魔術部のバイオレットちゃんだね？\nかっこいいね",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="…うん\n(すこし見せちゃおう)"},
  { ch=2, img="char2_new/glad" , text="イエイ～！ "},
  { ch=1, img="char6_new/normal" , text="…別に\n(超能力に頼りたくないな…)"},
  { ch=2, img="char2_new/normal" , text="あ、そうだ。\n実はアキも魔術できるんだよ"},
  { ch=2, img="char2_new/normal" , text="キュープの魔術だよ。\nつい先分ったんだ"},
  { ch=1, img="char6_new/anger" , text="ふん！\n(ボロ負かして二度と\nキュープしたくないしてやる！) "},

  { ch=1, text="" }
}

local act6_act3_ = {
  { ch=2, img="char3_new/glad" , text="…キュビート？\n面白い、\nキュープを制御するですね？",board_flip="H",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="ふん！\n(この人はもう何が掴んでるみたい)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" , text="おや？\nどうやら招かざる客ですね？",board_flip="V"},
  { ch=2, img="char3_new/glad" , text="いや、\n実はちょうどよかったですけど",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="やっぱり…！\n(この人もやっつけないと！)"},

  { ch=1, text="" }
}

local act6_act4_ = {
  { ch=1, img="char6_new/normal" , text="…\n(まずは\n近くの部室からチェックしよ)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char4_new/glad" , text="あら、今日の筆捌きは\nいつもより好調ですわ",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="おう？貴女は…魔術部の？ "},
  { ch=1, img="char6_new/normal" , text="…？\n(彼女は気づいたのか？) "},
  { ch=2, img="char4_new/normal" , text="なんに？\n貴女もワタクシのファンでこと？"},
  { ch=1, img="char6_new/normal" , text="…いえ。\n(どうやらただのバカだね)"},
  { ch=2, img="char4_new/glad" , text="ワタクシの幻想的な絵は\n貴女の魔術に負けませんわよ"},
  { ch=1, img="char6_new/anger" , text="…ふん！\n(お手並みを拝見しちゃおうかしら)"},

  { ch=1, text="" }
}

local act6_act5_ = {
  { ch=2, img="char5_new/normal" , text="今日は外が騒がしいだね",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char5_new/glad" , text="だがその位で心を\n乱すわけにはいかんだね！"},
  { ch=1, img="char6_new/normal" , text="…おい\n(ちょっと探ってみよう) ",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char5_new/anger" , text="雑念退散！集中修練！"},
  { ch=1, img="char6_new/glad" , text="…えい！\n(脅かしてやる！)"},
  { ch=2, img="char5_new/normal" , text="おう、お客さんだね"},
  { ch=2, img="char5_new/glad" , text="きっと心身鍛錬しに来たんだね？"},
  { ch=1, img="char6_new/normal" , text="…違う\n(意外と効いてない？)"},
  { ch=2, img="char5_new/glad" , text="成功は一日にしてならず、\n実力の裏は努力続けるしかない！"},
  { ch=2, img="char5_new/anger" , text="魔術みたいなわけには\nいかないだね！"},
  { ch=1, img="char6_new/anger" , text="ふん！\n(なら試してみよう！)"},

  { ch=1, text="" }
}

local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(今日も魔術の練習をしましょう)",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="えい！"},
  { ch=1, img="char6_new/sad" , text="(ダメ！\nまた超能力使っちゃった…)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(本当は手品で\nやるつもりなのに…)",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(いつもうっかりして\n超能力を使う…)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(もう一回しよう！) ",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="うッ！\n(痛い！) ",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=420, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},effect_w="size_M",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(何で\nまた超能力使っちゃったのよ…)"},
  { ch=1, img="char6_new/normal" , text="…え？\n(このキュープ達は？)",board_flip="V"},

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