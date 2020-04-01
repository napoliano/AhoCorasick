# AhoCorasick
The Aho-Corasick algorithm implemented with C++ and a simple example of use


Suppose there is a pattern matching machine for the set of keywords {he, she, his, hers}.
If you input 'hisheshehers', then the machine outputs 'his', 'she', 'he', 'she', 'he', 'he', 'hers'.

This is because:  
(his)heshehers  
hi(she)shehers  
his(he)shehers  
hishe(she)hers  
hishes(he)hers  
hisheshe(he)rs  
hisheshe(hers)  
