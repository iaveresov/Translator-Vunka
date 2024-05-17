#!/usr/bin/python3
import os.path
from os import path

first_for_G = dict({})
grammar_rules = dict({})


f = open("grammar.txt")

nonterms = []
terms = []

#start_nterm = f.readline()
#start_nterm = start_nterm[:len(start_nterm) - 1]
#nonterms = list(f.readline().split())
#terms = list(f.readline().split())
#nonterms.append("S")
#terms.append("$")

#grammar_rules["S"] = [[start_nterm]]

for line in f:
    rule_list = line[:len(line) - 1].split("::=")
    nonterms.append(rule_list[0])
    right_rules = rule_list[1].split("|")
    for right_rule in right_rules:
        if rule_list[0] in grammar_rules:
            grammar_rules[rule_list[0]].append(right_rule.split())
        else:
            grammar_rules.setdefault(rule_list[0], [(right_rule.split())])

for nterm in nonterms:
    for rule in grammar_rules[nterm]:
        for gsymb in rule:
            if gsymb not in nonterms:
                terms.append(gsymb)


start_nterm = nonterms[0]
nonterms.insert(0, "S")
terms.append("$")
grammar_rules["S"] = [[start_nterm]]

print("-------------------------------------------------")
print(nonterms)
print(terms)
print(grammar_rules)
print("-------------------------------------------------")

def first(seq):
    if seq == ["epsilon"]:
        return set({"epsilon"})
    
    if len(seq) == 1 and seq[0] in terms:
        return first_for_G[seq[0]]

    result = set({})
    
    for index in range(len(seq)):
        adset = first_for_G[seq[index]]
        if "epsilon" not in adset:
            return result.union(adset)
        elif index == len(seq) - 1 and "epsilon" in adset:
            return 
        else:
            result = result.union(adset.difference({"epsilon"}))

    return list(result)
       

def fill_first_for_G(first_for_G):
    for term in terms:
        first_for_G[term] = set({term})

    for nonterm in nonterms:
        first_for_G[nonterm] = {}
    
    update = True

    while update:
        update = False
        for head in grammar_rules:
            for rule in grammar_rules[head]:
                if rule == ["epsilon"]:
                    first_for_G[rule].add("epsilon")
                else:
                    first_length = len(first_for_G[head])
                    update_set = set(first_for_G[head])
                    update_set.update(first(rule))
                    first_for_G[head] = update_set
                    update_length = len(first_for_G[head])
                    
                    if update_length != first_length:
                        update = True

fill_first_for_G(first_for_G)

I0 = [["S", ["@", start_nterm], "$"]]

def closure(I):
    first_len = len(I)
    updated_len = len(I) - 1
    while first_len != updated_len:
        first_len = updated_len
        for item in I:
            item_rule = item[1]
            preview_char = item[2]
            dot_index = item_rule.index("@")
            if dot_index < len(item_rule) - 1 and item_rule[dot_index + 1] in nonterms:
                next_head = item_rule[dot_index + 1]
                for char in first(item_rule[dot_index + 2:] + [preview_char]):
                    for next_rule in grammar_rules[next_head]:
                        new_item = [next_head, ["@"] + next_rule, char]
                        if new_item not in I:
                            I.append(new_item)
        updated_len = len(I)
    return I


def goto(I, X):
    J = []
    for item in I:
        item_rule = item[1]
        item_head = item[0]
        preview_char = item[2]
        dot_index = item_rule.index("@")
        if dot_index < len(item_rule) - 1 and item_rule[dot_index + 1] == X:
            new_item = list(item_rule)
            new_item.remove("@")
            new_item.insert(dot_index + 1, "@")
            J.append([item_head, new_item, preview_char])
    return closure(J)



if path.exists("lr_table.cpp"):
    wf = open("lr_table.cpp", "w")
else:
    wf = open("lr_table.cpp", "x")

#generation .h file
wf.write("#include<iostream>" + "\n")
wf.write("#include<string>" + "\n")
wf.write("#include<utility>" + "\n")
wf.write("#include<vector>" + "\n")
wf.write("#include<map>")
wf.write("\n\n")
wf.write("using namespace std;")
wf.write("\n\n")

#generation tables and rules for reduce
table_action = [dict({})]
table_goto = [dict({})]

I0 = [["S", ["@", start_nterm], "$"]]
items = [closure(I0)]
reduce_rules = []

first_len = len(items)
updated_len = len(items) - 1
while first_len != updated_len:
    first_len = updated_len
    for current_stmt in range(len(items)):

        for X in nonterms:
            goto_set = goto(items[current_stmt], X)
            if len(goto_set) != 0:
                if goto_set not in items:
                    items.append(goto_set)
                    table_goto.append(dict({}))
                    table_action.append(dict({}))

                goto_stmt = items.index(goto_set)
                table_goto[current_stmt][X] = goto_stmt
            else:
                table_goto[current_stmt][X] = -1

        for X in terms:
            new_set = goto(items[current_stmt], X)
            if len(new_set) == 0:
                reduce = False
                for item in items[current_stmt]:
                    if item[2] == X and item[1].index("@") ==  len(item[1]) - 1:
                        reduce = True
                        if item[0] == "S" and X == "$":
                            table_action[current_stmt][X] = ["acc", -1]
                        else:
                            new_rule = list(item[1])
                            new_rule.remove("@")
                            if [item[0], new_rule] not in reduce_rules:
                                reduce_rules.append([item[0], new_rule])
                            rule_ind = reduce_rules.index([item[0], new_rule])
                            if X not in table_action[current_stmt].keys():
                                table_action[current_stmt][X] = ["r", rule_ind]
                            elif table_action[current_stmt][X][0] == "s":
                                print("conflict")

                if not reduce:
                    table_action[current_stmt][X] = ["err", -1]
            else:
                if new_set not in items:
                    items.append(new_set)
                    table_action.append(dict({}))
                    table_goto.append(dict({}))
                new_stmt = items.index(new_set)
                table_action[current_stmt][X] = ["s", new_stmt]
    updated_len = len(items)

for string in table_action:
    print(string)
print("---------------------------------------")
print(len(table_action))
print(len(table_goto))
print("---------------------------------------")
for string in table_goto:
    print(string)

print(reduce_rules)

#write table ACTION in cpp file
wf.write("vector< vector< pair<string, pair<string, int> > >> tableAction = {" + '\n')
for string in table_action:
    wf.write("\t" + "{")
    for char in string.keys():
        wf.write("{" + f'"{char}", ' + "{ ")
        for act in string[char]:
            if type(act) == str:
                wf.write(f'"{act}", ')
            else:
                wf.write(f'{act}, ')
        wf.write("}}, ")
    wf.write("}, " + '\n')
wf.write("};" +'\n\n')

#write table GOTO in cpp file
wf.write("vector< vector< pair<string, int> > > gotoTable = {\n")
for string in table_goto:
    wf.write("\t" + "{")
    for char in string.keys():
        wf.write("{" + f'"{char}", ' + f'{string[char]}' + "}, ")
    wf.write("}, " + '\n')
wf.write("};" + '\n\n')

#write reduce rules in cpp file
wf.write("vector< pair< string, vector<string> > > reduceRules = {\n")
for rule in reduce_rules:
    wf.write('\t' + "{" + f'"{rule[0]}", ' + '\n\t\t' + "{ ")
    for char in rule[1]:
        wf.write(f'"{char}", ')
    wf.write("}}, " + '\n')
wf.write("};" + '\n\n')
'''
def make_items():
    I0 = [["S", ["@", start_nterm], "$"]]
    items = [closure(I0)]
    first_len = len(items)
    updated_len = len(items) - 1
    while first_len != updated_len:
        first_len = updated_len
        for item_set in items:
            for X in nonterms:
                new_set = goto(item_set, X)
                if len(new_set) != 0 and new_set not in items:
                    items.append(new_set)
            for X in terms:
                new_set = goto(item_set, X)
                if len(new_set) != 0 and new_set not in items:
                    items.append(new_set)
        updated_len = len(items)
    return items

I0 = closure(I0)
items = make_items()
index = 0
for i in items:
    print(index, ": ")
    for j in i:
        print(j[0]," ->", "".join(j[1]), "," ,j[2])
    print()
    index += 1

if path.exists("lr_table.cpp"):
    wf = open("lr_table.cpp", "w")
else:
    wf = open("lr_table.cpp", "x")

#generation .h file
wf.write("#include<iostream>" + "\n")
wf.write("#include<string>" + "\n")
wf.write("#include<utility>" + "\n")
wf.write("#include<vector>" + "\n")
wf.write("#include<map>")
wf.write("\n\n")
wf.write("using namespace std;")
wf.write("\n\n")

#ACTION table generation
wf.write("vector< vector< pair<string, pair<string, int> > >> tableAction = {" + '\n')
reduce_rules = []
for index in range(len(items)):
    wf.write("\t{")
    for term in terms:
        new_stmt = goto(items[index], term)
        if new_stmt != []:
            new_index = items.index(new_stmt)
            wf.write("{" + f'"{term}"' + ", " + "{" + '"s", ' + f'{new_index}' + "}" + "}, ")
            #wf.write("{" + f'"{term}"' + ", " + f'"s{new_index}"' + "}, ")
        elif term == "$" and ["S", [start_nterm, "@"], "$"] in items[index]:
            wf.write("{" + '"$", ' + '{"acc", -1}' + "}, ")
        else:
            reduce = False
            for item in items[index]:
                item_head = item[0]
                item_rule = list(item[1])
                preview_symbol = item[2]
                dot_index = item_rule.index("@")
                if dot_index == len(item_rule) - 1 and preview_symbol == term:
                    reduce = True
                    item_rule.remove("@")
                    if [item_head, item_rule] not in reduce_rules:
                        reduce_rules.append([item_head, item_rule])
                    wf.write("{" + f'"{term}", ' + "{" + '"r", ' f'{reduce_rules.index([item_head, item_rule])}' + "}" + "}, ")
                    #wf.write("{" + f'"{term}", ' + f'"r{reduce_rules.index([item_head, item_rule]) + 1}"' + "}, ")

                    break
            if not reduce:
                wf.write("{" + f'"{term}", ' + '{"err", -1}' + "}, ")
    wf.write("}, "+'\n')
wf.write("};\n\n")

#generation grammar to reduce
wf.write("vector< pair< string, vector<string> > > reduceRules = {\n")
for rule in reduce_rules:
    wf.write("\t{" + f'"{rule[0]}",\n' + "\t\t{")
    for symb in rule[1]:
        wf.write(f'"{symb}", ')
    wf.write("}},\n")
wf.write("};\n\n")

#generation GOTO table
wf.write("vector< vector< pair<string, int> > > gotoTable = {\n")
for index in range(len(items)):
    wf.write("\t{")
    for nterm in nonterms:
        if nterm != "S":
            new_stmt = goto(items[index], nterm)
            if new_stmt != []:
                new_index = items.index(new_stmt)
                wf.write("{" + f'"{nterm}", ' + f'{new_index}' + "}, ")
            else:
                wf.write("{" + f'"{nterm}", ' + f'{-1}' + "}, ")
    wf.write('},\n')
wf.write("};\n")

wf.close()
'''