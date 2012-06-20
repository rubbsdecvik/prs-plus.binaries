#include "desaturate.h"

/** 
* Converts character to lower case, removing accents
*/
uint16_t desaturate(uint16_t ch) {
	/*
		40 - 70, standard latin range

		U+		0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F	
		0040	@	A	B	C	D	E	F	G	H	I	J	K	L	M	N	O
		0050	P	Q	R	S	T	U	V	W	X	Y	Z	[	\	]	^	_
		0060	`	a	b	c	d	e	f	g	h	i	j	k	l	m	n	o
		0070	p	q	r	s	t	u	v	w	x	y	z	{	|	}	~	DEL	
	*/
	if (ch > 0x40 && ch < 0x5b) {
		// convert to lower case
		return ch + 0x20;
	}
	if (ch > 0x60 && ch < 0x7b) {
		// char is already lower case
		return ch;
	}


	/*
		80–FF, C1 Controls and Latin-1 Supplement
	*/
	if (ch >= 0xc0 && ch <= 0x240) {
		uint16_t map[] = {
			//U+	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F 
			//00C0	À	Á	Â	Ã	Ä	Å	Æ	Ç	È	É	Ê	Ë	Ì	Í	Î	Ï  TODO: Æ
				   'a','a','a','a','a','a','a','c','e','e','e','e','i','i','i','i',
			//00D0	Ð	Ñ	Ò	Ó	Ô	Õ	Ö	×	Ø	Ù	Ú	Û	Ü	Ý	Þ	ß  TODO: ß
				   'd','n','o','o','o','o','o','x','o','u','u','u','u','y','p','s',
			//00E0	à	á	â	ã	ä	å	æ	ç	è	é	ê	ë	ì	í	î	ï  TODO: æ
				   'a','a','a','a','a','a','a','c','e','e','e','e','i','i','i','i',
			//00F0	ð	ñ	ò	ó	ô	õ	ö	÷	ø	ù	ú	û	ü	ý	þ	ÿ
				   'o','n','o','o','o','o','o', 0 , 0 ,'u','u','u','u','y','d','y',
			//0100	Ā	ā	Ă	ă	Ą	ą	Ć	ć	Ĉ	ĉ	Ċ	ċ	Č	č	Ď	ď	Latin Extended-A 0100–017F
				   'a','a','a','a','a','a','c','c','c','c','c','c','c','c','d','d',
			//0110	Đ	đ	Ē	ē	Ĕ	ĕ	Ė	ė	Ę	ę	Ě	ě	Ĝ	ĝ	Ğ	ğ
				   'd','d','e','e','e','e','e','e','e','e','e','e','g','g','g','g',
			//0120	Ġ	ġ	Ģ	ģ	Ĥ	ĥ	Ħ	ħ	Ĩ	ĩ	Ī	ī	Ĭ	ĭ	Į	į
				   'g','g','g','g','h','h','h','h','i','i','i','i','i','i','i','i',
			//0130	İ	ı	Ĳ	ĳ	Ĵ	ĵ	Ķ	ķ	ĸ	Ĺ	ĺ	Ļ	ļ	Ľ	ľ	Ŀ TODO Ĳ,ĳ,Ŀ
				   'i','l','i','i','j','j','k','k','k','l','i','l','l','l','i','l',
			//0140	ŀ	Ł	ł	Ń	ń	Ņ	ņ	Ň	ň	ŉ	Ŋ	ŋ	Ō	ō	Ŏ	ŏ TODO Ŋ,ŋ
				   'l','l','l','n','n','n','n','n','n','n','n','n','o','o','o','o',
			//0150	Ő	ő	Œ	œ	Ŕ	ŕ	Ŗ	ŗ	Ř	ř	Ś	ś	Ŝ	ŝ	Ş	ş TODO Œ,œ
				   'o','o','o','o','r','r','r','r','r','r','s','s','s','s','s','s',		
			//0160	Š	š	Ţ	ţ	Ť	ť	Ŧ	ŧ	Ũ	ũ	Ū	ū	Ŭ	ŭ	Ů	ů
				   's','s','t','t','t','t','t','t','u','u','u','u','u','u','u','u',		
			//0170	Ű	ű	Ų	ų	Ŵ	ŵ	Ŷ	ŷ	Ÿ	Ź	ź	Ż	ż	Ž	ž	ſ
				   'u','u','u','u','w','w','y','y','y','z','z','z','z','z','z','r',		
			//0180	ƀ	Ɓ	Ƃ	ƃ	Ƅ	ƅ	Ɔ	Ƈ	ƈ	Ɖ	Ɗ	Ƌ	ƌ	ƍ	Ǝ	Ə	Latin Extended-B 0180–024F
				   'b','b','b','b','b','b','c','c','c','d','d','b','b','d','e','e',		
			//0190	Ɛ	Ƒ	ƒ	Ɠ	Ɣ	ƕ	Ɩ	Ɨ	Ƙ	ƙ	ƚ	ƛ	Ɯ	Ɲ	ƞ	Ɵ
				   'e','f','f','g','y','i','l','f','k','k','f','a','s','n','n','o',	
			//01A0	Ơ	ơ	Ƣ	ƣ	Ƥ	ƥ	Ʀ	Ƨ	ƨ	Ʃ	ƪ	ƫ	Ƭ	ƭ	Ʈ	Ư
					0,  0,  0,  0,  0,  0, 'r','s','s','s', 0, 't','t','t','t','v',
			//01B0	ư	Ʊ	Ʋ	Ƴ	ƴ	Ƶ	ƶ	Ʒ	Ƹ	ƹ	ƺ	ƻ	Ƽ	ƽ	ƾ	ƿ
				   'u','u','u','y','y','z','z', 0,  0,  0,  0, 'z', 0,  0,  0, 'p',
			//01C0	ǀ	ǁ	ǂ	ǃ	Ǆ	ǅ	ǆ	Ǉ	ǈ	ǉ	Ǌ	ǋ	ǌ	Ǎ	ǎ	Ǐ
					0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 'a','a','i',
			//01D0	ǐ	Ǒ	ǒ	Ǔ	ǔ	Ǖ	ǖ	Ǘ	ǘ	Ǚ	ǚ	Ǜ	ǜ	ǝ	Ǟ	ǟ
				   'i','o','o','u','u','u','u','u','u','u','u','u','u','e','a','a',
			//01E0	Ǡ	ǡ	Ǣ	ǣ	Ǥ	ǥ	Ǧ	ǧ	Ǩ	ǩ	Ǫ	ǫ	Ǭ	ǭ	Ǯ	ǯ TODO Ǣ,ǣ
				   'a','a','a','a','g','g','g','g','k','k','q','q','q','q','e','e',
			//01F0	ǰ	Ǳ	ǲ	ǳ	Ǵ	ǵ	Ƕ	Ƿ	Ǹ	ǹ	Ǻ	ǻ	Ǽ	ǽ	Ǿ	ǿ TODO Ǽ,ǽ
				   'j', 0 , 0 , 0 ,'g','g', 0 ,'p','n','n','a','a','a','a','o','o',
			//0200	Ȁ	ȁ	Ȃ	ȃ	Ȅ	ȅ	Ȇ	ȇ	Ȉ	ȉ	Ȋ	ȋ	Ȍ	ȍ	Ȏ	ȏ
				   'a','a','a','a','e','e','e','e','i','i','i','i','o','o','o','o',
			//0210	Ȑ	ȑ	Ȓ	ȓ	Ȕ	ȕ	Ȗ	ȗ	Ș	ș	Ț	ț	Ȝ	ȝ	Ȟ	ȟ
				   'r','r','r','r','u','u','u','u','s','s','t','t','e','e','h','h',
			//0220	Ƞ	ȡ	Ȣ	ȣ	Ȥ	ȥ	Ȧ	ȧ	Ȩ	ȩ	Ȫ	ȫ	Ȭ	ȭ	Ȯ	ȯ TODO Ƞ, ȡ
				   'n','d','r','r','u','u','u','u','s','s','t','t','e','e','h','h',
			//0230	Ȱ	ȱ	Ȳ	ȳ	ȴ	ȵ	ȶ	ȷ	ȸ	ȹ	Ⱥ	Ȼ	ȼ	Ƚ	Ⱦ	ȿ
				   'o','o','y','y','l','n','t','j','f','f','a','c','c','t','t','s',
			//0240	ɀ	Ɂ	ɂ	Ƀ	Ʉ	Ʌ	Ɇ	ɇ	Ɉ	ɉ	Ɋ	ɋ	Ɍ	ɍ	Ɏ	ɏ
				   'z','c','c','b','a','a','e','e','j','j','q','q','r','f','a','a'
		};

		uint16_t tmp = map[ch - 0xc0];
		if (tmp != 0) {
			return tmp;
		}
	}


	/*
		Latin Extended Additional 1E00–1EFF
	*/
	if (ch >= 0x1e00 && ch <= 0x1eff) {
		uint16_t map[] = {
			//U+		0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F 
			//  1E00	Ḁ	ḁ	Ḃ	ḃ	Ḅ	ḅ	Ḇ	ḇ	Ḉ	ḉ	Ḋ	ḋ	Ḍ	ḍ	Ḏ	ḏ	
					   'a','a','b','b','b','b','b','b','c','c','d','d','d','d','d','d',
			//	1E10	Ḑ	ḑ	Ḓ	ḓ	Ḕ	ḕ	Ḗ	ḗ	Ḙ	ḙ	Ḛ	ḛ	Ḝ	ḝ	Ḟ	ḟ
					   'd','d','d','d','e','e','e','e','e','e','e','e','e','e','f','f',
			//	1E20	Ḡ	ḡ	Ḣ	ḣ	Ḥ	ḥ	Ḧ	ḧ	Ḩ	ḩ	Ḫ	ḫ	Ḭ	ḭ	Ḯ	ḯ
					   'g','g','h','h','h','h','h','h','h','h','h','h','i','i','i','i',
			//	1E30	Ḱ	ḱ	Ḳ	ḳ	Ḵ	ḵ	Ḷ	ḷ	Ḹ	ḹ	Ḻ	ḻ	Ḽ	ḽ	Ḿ	ḿ
					   'k','k','k','k','k','k','l','l','l','l','l','l','l','l','m','m',
			//	1E40	Ṁ	ṁ	Ṃ	ṃ	Ṅ	ṅ	Ṇ	ṇ	Ṉ	ṉ	Ṋ	ṋ	Ṍ	ṍ	Ṏ	ṏ
					   'm','m','m','m','n','n','n','n','n','n','n','n','o','o','o','o',
			//	1E50	Ṑ	ṑ	Ṓ	ṓ	Ṕ	ṕ	Ṗ	ṗ	Ṙ	ṙ	Ṛ	ṛ	Ṝ	ṝ	Ṟ	ṟ
					   'o','o','o','o','p','p','p','p','r','r','r','r','r','r','r','r',
			//	1E60	Ṡ	ṡ	Ṣ	ṣ	Ṥ	ṥ	Ṧ	ṧ	Ṩ	ṩ	Ṫ	ṫ	Ṭ	ṭ	Ṯ	ṯ
					   's','s','s','s','s','s','s','s','s','s','t','t','t','t','t','t',
			//	1E70	Ṱ	ṱ	Ṳ	ṳ	Ṵ	ṵ	Ṷ	ṷ	Ṹ	ṹ	Ṻ	ṻ	Ṽ	ṽ	Ṿ	ṿ
					   't','t','u','u','u','u','u','u','u','u','u','u','v','v','v','v',
			//	1E80	Ẁ	ẁ	Ẃ	ẃ	Ẅ	ẅ	Ẇ	ẇ	Ẉ	ẉ	Ẋ	ẋ	Ẍ	ẍ	Ẏ	ẏ
					   'w','w','w','w','w','w','w','w','w','w','x','x','x','x','y','y',
			//	1E90	Ẑ	ẑ	Ẓ	ẓ	Ẕ	ẕ	ẖ	ẗ	ẘ	ẙ	ẚ	ẛ	ẜ	ẝ	ẞ	ẟ
					   'z','z','z','z','z','z','h','t','w','y','a','r', 0,  0,  0,  0,
			//	1EA0	Ạ	ạ	Ả	ả	Ấ	ấ	Ầ	ầ	Ẩ	ẩ	Ẫ	ẫ	Ậ	ậ	Ắ	ắ
					   'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			//	1EB0	Ằ	ằ	Ẳ	ẳ	Ẵ	ẵ	Ặ	ặ	Ẹ	ẹ	Ẻ	ẻ	Ẽ	ẽ	Ế	ế
					   'a','a','a','a','a','a','a','a','e','e','e','e','e','e','e','e',
			//	1EC0	Ề	ề	Ể	ể	Ễ	ễ	Ệ	ệ	Ỉ	ỉ	Ị	ị	Ọ	ọ	Ỏ	ỏ
					   'e','e','e','e','e','e','e','e','i','i','i','i','o','o','o','o',
			//	1ED0	Ố	ố	Ồ	ồ	Ổ	ổ	Ỗ	ỗ	Ộ	ộ	Ớ	ớ	Ờ	ờ	Ở	ở
					   'o','o','o','o','o','o','o','o','o','o','a','o','o','o','o','o',
			//	1EE0	Ỡ	ỡ	Ợ	ợ	Ụ	ụ	Ủ	ủ	Ứ	ứ	Ừ	ừ	Ử	ử	Ữ	ữ
					   'd','d','o','o','u','u','u','u','u','u','u','u','u','u','u','u',
			//	1EF0	Ự	ự	Ỳ	ỳ	Ỵ	ỵ	Ỷ	ỷ	Ỹ	ỹ	Ỻ	ỻ	Ỽ	ỽ	Ỿ	ỿ
					   'v','v','y','y','y','y','y','y','y','y', 0,  0,  0,  0,  0,  0
		};

		uint16_t tmp = map[ch - 0x1e00];
		if (tmp != 0) {
			return tmp;
		}

	}

	/*
		Cyrrilic 0x400 - 0x45f

		U+		0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F 
		U+040x	Ѐ	Ё	Ђ	Ѓ	Є	Ѕ	І	Ї	Ј	Љ	Њ	Ћ	Ќ	Ѝ	Ў	Џ
		U+041x	А	Б	В	Г	Д	Е	Ж	З	И	Й	К	Л	М	Н	О	П
		U+042x	Р	С	Т	У	Ф	Х	Ц	Ч	Ш	Щ	Ъ	Ы	Ь	Э	Ю	Я
		U+043x	а	б	в	г	д	е	ж	з	и	й	к	л	м	н	о	п
		U+044x	р	с	т	у	ф	х	ц	ч	ш	щ	ъ	ы	ь	э	ю	я
		U+045x	ѐ	ё	ђ	ѓ	є	ѕ	і	ї	ј	љ	њ	ћ	ќ	ѝ	ў	џ
	*/
	if (ch >= 0x400 && ch < 0x410) {
		return ch + 0x10*5;
	}
	if (ch >= 0x410 && ch < 0x430){
		return ch + 0x10*2;
	}

	return ch;
}