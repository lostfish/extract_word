Extract words from the corpus without priori knowledge
===================================

抽词算法
------------
参考：http://www.matrix67.com/blog/archives/5044

基本思想
---------
利用字片段的内聚程度和自由运用程度来度量字片段是否构成词

内聚程度:字片段内所有不同切分算出的互信息的最小值 (p(ab)/p(a)p(b))

自由运用程度:字片段左邻字和右邻字信息熵的较小值

- extract_by_scan.cpp 直接扫描语料抽取新词
- extract_by_suffix.cpp 扫描语料建立后缀数组再抽取新词，可利用lcp剪枝

TODO
--------
模块化重构
