#! /usr/bin/env python
#encoding: utf-8

import sys
import re

puncs = '，。；？：”“‘’（）『』【】\'",.;-+%<>?[]{}()'.decode('utf-8')
n_punc = len(puncs)
punc_map = dict([(ch, i ) for i, ch in enumerate(puncs)])

def extract_text_feature(text, charset='utf-8'):
    '''
    字符数，是否含有标点，数字，字母, 位置
    '''
    a = text.decode(charset, 'ignore')
    length = len(a)
    vec = [0 for i in range(n_punc)]
    has_digit = 0
    has_alpha = 0
    for ch in a:
        if ch in punc_map:
            index = punc_map[ch]
            vec[index] = 1
        if not has_alpha and ch.isalpha():
            has_alpha = 1
        if not has_digit and ch.isdigit():
            has_digit = 1
    vec.append(has_alpha)
    vec.append(has_digit)
    vec.append(length)
    return vec

def vectorize(filepath, common_word_file = None):
    common_words = set([line.strip() for line in file(common_word_file)])
    with open(filepath) as f:
        for line in f:
            s = line.rstrip().split('\t')
            if len(s) != 4:
                continue
            word = s[0]
            label = 0
            if word in common_words:
                label = 1
            freendom = float(s[1])
            cohesion = float(s[2])
            freq = int(s[3])
            vec =  extract_text_feature(word)
            vec.append(freendom)
            vec.append(cohesion)
            vec.append(freq)
            str_vec = ["%d:%d" % (i+1, e) for i,e in enumerate(vec)]
            print "%d %s" % (label, ' '.join(str_vec))

candi_word_file = sys.argv[1]
common_word_file = sys.argv[2]
vectorize(candi_word_file, common_word_file)
