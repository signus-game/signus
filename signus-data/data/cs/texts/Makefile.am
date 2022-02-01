texts_src = brief1.txt brief10.txt brief11.txt brief12.txt brief13.txt \
	brief14.txt brief15.txt brief16.txt brief17.txt brief18.txt \
	brief19.txt brief2.txt brief3.txt brief4.txt brief5.txt brief6.txt \
	brief7.txt brief8.txt brief9.txt br1_001.txt br1_002.txt br2_001.txt \
	br4_001.txt br5_001.txt br6_001.txt br7_001.txt br8_001.txt \
	br9_001.txt b10_001.txt b11_001.txt b12_001.txt b13_001.txt \
	b14_001.txt b15_001.txt b16_001.txt b17_001.txt b18_001.txt \
	b19_001.txt credits.txt debrf1.txt debrf10.txt debrf11.txt \
	debrf12.txt debrf13.txt debrf14.txt debrf15.txt debrf16.txt \
	debrf17.txt debrf18.txt debrf19.txt debrf2.txt debrf3.txt debrf4.txt \
	debrf5.txt debrf6.txt debrf7.txt debrf8.txt debrf9.txt err0.txt \
	err1.txt err2.txt err3.txt err4.txt err5.txt err6.txt err7.txt \
	help0.txt help1.txt help10.txt help2.txt help3.txt help4.txt \
	help5.txt help6.txt help7.txt help8.txt help9.txt msg0.txt msg1.txt \
	msg10.txt msg11.txt msg2.txt msg3.txt msg4.txt msg5.txt msg6.txt \
	msg7.txt msg8.txt msg9.txt objec1.txt objec10.txt objec11.txt \
	objec12.txt objec13.txt objec14.txt objec15.txt objec16.txt \
	objec17.txt objec18.txt objec19.txt objec2.txt objec3.txt objec4.txt \
	objec5.txt objec6.txt objec7.txt objec8.txt objec9.txt txt0.txt \
	txt1.txt txt10.txt txt11.txt txt12.txt txt13.txt txt14.txt txt15.txt \
	txt16.txt txt17.txt txt18.txt txt19.txt txt2.txt txt20.txt txt21.txt \
	txt22.txt txt23.txt txt24.txt txt25.txt txt26.txt txt27.txt txt28.txt \
	txt29.txt txt3.txt txt30.txt txt31.txt txt32.txt txt33.txt txt34.txt \
	txt35.txt txt36.txt txt37.txt txt38.txt txt39.txt txt4.txt txt40.txt \
	txt41.txt txt42.txt txt43.txt txt44.txt txt45.txt txt46.txt txt47.txt \
	txt48.txt txt49.txt txt5.txt txt50.txt txt51.txt txt52.txt txt53.txt \
	txt54.txt txt55.txt txt56.txt txt57.txt txt58.txt txt59.txt txt6.txt \
	txt60.txt txt61.txt txt62.txt txt63.txt txt64.txt txt65.txt txt66.txt \
	txt67.txt txt68.txt txt69.txt txt7.txt txt70.txt txt71.txt txt73.txt \
	txt74.txt txt75.txt txt76.txt txt77.txt txt78.txt txt79.txt txt8.txt \
	txt80.txt txt81.txt txt82.txt txt83.txt txt84.txt txt85.txt txt9.txt \
	udes1.txt \
	udes10.txt udes11.txt udes12.txt udes13.txt udes14.txt udes15.txt \
	udes16.txt udes17.txt udes18.txt udes19.txt udes2.txt udes20.txt \
	udes21.txt udes22.txt udes23.txt udes24.txt udes25.txt udes26.txt \
	udes27.txt udes28.txt udes29.txt udes3.txt udes30.txt udes31.txt \
	udes32.txt udes33.txt udes34.txt udes35.txt udes36.txt udes37.txt \
	udes38.txt udes39.txt udes4.txt udes40.txt udes41.txt udes42.txt \
	udes43.txt udes44.txt udes45.txt udes5.txt udes50.txt udes51.txt \
	udes52.txt udes53.txt udes54.txt udes55.txt udes56.txt udes57.txt \
	udes58.txt udes6.txt udes7.txt udes8.txt udes9.txt unam1.txt \
	unam10.txt unam11.txt unam12.txt unam13.txt unam14.txt unam15.txt \
	unam16.txt unam17.txt unam18.txt unam19.txt unam2.txt unam20.txt \
	unam21.txt unam22.txt unam23.txt unam24.txt unam25.txt unam26.txt \
	unam27.txt unam28.txt unam29.txt unam3.txt unam30.txt unam31.txt \
	unam32.txt unam33.txt unam34.txt unam35.txt unam36.txt unam37.txt \
	unam38.txt unam39.txt unam4.txt unam40.txt unam41.txt unam42.txt \
	unam43.txt unam44.txt unam45.txt unam5.txt unam50.txt unam51.txt \
	unam52.txt unam53.txt unam54.txt unam55.txt unam56.txt unam57.txt \
	unam58.txt unam59.txt unam6.txt unam60.txt unam7.txt unam8.txt \
	unam9.txt wpnnm0.txt wpnnm1.txt wpnnm10.txt wpnnm11.txt wpnnm12.txt \
	wpnnm2.txt wpnnm3.txt wpnnm4.txt wpnnm5.txt wpnnm6.txt wpnnm7.txt \
	wpnnm8.txt wpnnm9.txt

dist_noinst_DATA = $(texts_src)

txtdir = $(datadir)/signus/$(VERSION)/cs

txt_DATA = texts.dat

texts.dat: $(texts_src)
	$(MAKEDAT) $@ $^


clean-local:
	rm -f texts.dat
