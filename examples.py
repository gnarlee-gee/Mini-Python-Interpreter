####tc1.py#######
a=[3]
b=[5]
l1=a+b
print(l1) 
>>>[3,5]

####tc2.py#######
x=['s1']
y=['s2']
z=['s3']
l2=(x+y+z)
print(l2)
>>>['s1','s2','s3']

####tc3.py#######
a=[1]
b=[2]
c=[3]
d=[4]
l3=a+b+c
l4=c+d+[5]
print(l3[0])
>>>1
print(l4[1:])
>>>[4,5]

####tc4.py#######
x=['a']
y=['b']
z=['c']
l5=[3]+[5]
print(l5)
>>>[3,5]
l6=(x+y+z)+['d']+['e']
print(l6)
>>>['a','b','c','d','e']
print(l6[1:])
>>>['b','c','d','e']

####tc5.py#######
w=3
m=[w]
y=0 if w>0 else -1
n=[y]
l7=m+n
print(l7)
>>>[3,0]


######tc6.py######
l1= ["st1"]
l2= l1 + ["st2"] + ["st3"] + ["ab"] + ["bc"]
print(l2)
>>>["st1", "st2","st3","ab","bc"]
print(l2[1:])
>>>["st2", "st3", "ab", "bc"]

l3=["ef"]
l4=["de"]
l5=l2 + l3 + l4
print(l5)
>>>["st1", "st2","st3","ab","bc","ef", "de"]



#######tc7.py#####
t=0
l1=[1]
l2=[2]
l3=[4]
l5=["st1"]
l6=["st2"]
l7=[5]
l8=[6]
l9=["st3"]

l10=l1+l2+l3+l7+l8
print(l10[1:])
>>>[2,4,5,6]

l11=l5 + l6 +l9
print(l11)
>>>["st1","st2","st3"]


#######tc8.py#######
t=0
a=1 if t>0 else 1
l1=[a]

b=2 if a>1 else 3
l2=[b]

l3=l1+l2+[2]+[3]+[4]

print(l3[1:])
>>>[3,2,3,4]

#########tc9.py#######

l1=[1]
l2=[2]
l3=[3]
a=11
b=12
c=13
l4=l1+l2+l3+[4]+[5]+[6]+[7]+[a]+[b]+[c]

print(l4)
>>>[1,2,3,4,5,6,7,11,12,13]


#########tc10.py#########
l1=["a"]
l2=["b"]
l3=["c"]
t=1
x="d" 
y="f"
z="g"
l4=l1+l2+l3+["o"]+["p"]+["q"]+[x]+[y]+[z]

print(l4)
>>>["a","b","c","o","p","q","d","f","g"]

##### tc11.py #####
a=40
b=400
m=4000 if b<a else 4
l1=[m]
l2=l1+l1+l1
print(l2[2]) #the index will always be given as len(list)-1
>>>4

##### tc12.py #####
x="a"
y="b"
z="z" if x==y else "c"
l1=[z]+["z"]

w="w" if x<y else "d"
l2=l1+[w]
print(l2)
>>>["c","z","w"]

##### tc13.py #####
a1=30
b1=50
c1=55

m1=22 if a1==30 else 0
l1=([a1]+[b1]+[c1])+[m1] 
print(l1[1:])
>>>[50,55,22]

b1=40
n1=34 if b1>45 else 0
l2=[n1]+l1
print(l2)
>>>[0,30,50,55,22]
print(l2[3])
>>>55

##### tc14.py (advanced) #####
a=40
l1=["50"]
#error <-(print this in .out file)
l2=[a]+l1 
l3=l1+(["300"]+["21"])
print(l3)
>>>["50","300","21"]
#error <-(print this in .out file)
l4=l3+[a]

##### tc15.py (advance- maximum 1 or 0 final test will be like this) #####
l1=[4]
l1=l1+[7]
l1=l1+[7]
l1=l1+[9]
l1=l1+[10]
print(l1)
>>>[4,7,7,9,10]
print(l1[2]) 
>>>7
l2=l1+l1+l1
print(l2[1:])
>>>[7,7,9,10,4,7,7,9,10,4,7,7,9,10]
