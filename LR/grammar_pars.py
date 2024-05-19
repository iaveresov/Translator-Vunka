#!/usr/bin/python3
import os.path
from os import path

first_for_G = dict({})
grammar_rules = dict({})


f = open("grammar.txt")

nonterms = []
terms = []


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
            if gsymb not in nonterms and gsymb not in terms and gsymb != "epsilon":
                terms.append(gsymb)


start_nterm = nonterms[0]
nonterms.insert(0, "S")
terms.append("correct")
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
            return result.union(adset)
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
                    first_for_G[head].add("epsilon")
                else:
                    first_length = len(first_for_G[head])
                    update_set = set(first_for_G[head])
                    update_set.update(first(rule))
                    first_for_G[head] = update_set
                    update_length = len(first_for_G[head])
                    
                    if update_length != first_length:
                        update = True

fill_first_for_G(first_for_G)
print("-----------------------")
for key in first_for_G:
    print(key, first_for_G[key])
print("-----------------------")
I0 = [["S", ["@", start_nterm], "correct"]]

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
                        if next_rule == ["epsilon"]:
                            empty_item = [next_head, ["@"], char]
                            if empty_item not in I:
                                I.append(empty_item)
                        else:
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



#generation tables and rules for reduce
table_action = [dict({})]
table_goto = [dict({})]

I0 = [["S", ["@", start_nterm], "correct"]]
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
                    if item[2] == X and item[1].index("@") == len(item[1]) - 1:
                        reduce = True
                        if item[0] == "S" and X == "correct":
                            table_action[current_stmt][X] = ["acc", -1]
                        else:
                            new_rule = list(item[1])
                            new_rule.remove("@")
                            if len(new_rule) == 0:
                                new_rule = ["epsilon"]
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


print("---------------------------------------")
print(len(table_action))
print(len(table_goto))
print("---------------------------------------")
print(reduce_rules)
print("---------------------------------------")


#write table ACTION in cpp file
with open("tableAction.txt", "w") as wf:
    print(len(table_action), file=wf)
    for string in table_action:
        print(len(string), file=wf, end=" ")
        for char in string.keys():
            print(char, *string[char], file=wf, end=" ")
        print(file=wf)

#write table GOTO in cpp file
with open("gotoTable.txt", "w") as wf:
    print(len(table_goto), file=wf)
    for string in table_goto:
        print(len(string), file=wf, end=" ")
        for char in string.keys():
            print(char, string[char], file=wf, end=" ")
        print(file=wf)

#write reduce rules in cpp file
with open("reduceRules.txt", "w") as wf:
    print(len(reduce_rules), file=wf)
    for rule in reduce_rules:
        print(rule[0], len(rule[1]), *rule[1], file=wf)

print(len(items))
print(len(table_action))
print(len(table_goto))