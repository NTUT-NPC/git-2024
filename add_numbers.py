def add(a, b):
    return a + b

# Example usage
num1 = 3
num2 = 5
print(f"The sum of {num1} and {num2} is {add(num1, num2)}")

def compare(kind: dict, number: list):
        flush = False
        straight = False
        count = 0
        if 5 in kind.values():
            flush =True
        for i in range(13):
            # print(number[i:i+5])
            if [1, 1, 1, 1, 1] == number[i:i+5]:
                straight =True
        # print(flush, straight)
        if flush and straight:
            return 9
        elif 4 in number:
            return 8
        elif 3 in number and 2 in number:
            return 7
        elif flush:
            return 6
        elif straight:
            return 5
        elif 3 in number:
            return 4
        elif 2 in number:
            for i in range(13):
                if 2 == number[i]:
                    count += 1
            if count == 2:
                return 3
            else:
                return 2
        else:
            return 1
             
def main():
    card = input().split(' ')
    kind = {'S':0, 'H':0, 'D':0, 'C':0}
    number = [0 for i in range(22)]
    card.sort()
    dupl = False
    for error in range(len(card)-1):
        if card[error] == card[error+1]:
            dupl = True
    for i in range(len(card)):
        card[i] = list(card[i])
    for check in range(len(card)):
        #print(card[check])
        try:
            kind[card[check].pop()] += 1
        except:
            print("Error input")
            exit()
        if len(card[check]) == 2:
            if not(card[check][1] == '0' and card [check][0] == '1'):
                #print("test")
                print("Error input")
                exit()
            number[9] += 1
        elif card[check][0] == 'A':
            number[0] +=1
            number[13] +=1
        elif  card[check][0] == 'J':
            number[10] +=1
        elif card[check][0] == 'Q':
            number[11] +=1
        elif card[check][0] == 'K':
            number[12] +=1
        elif ord('2') <= ord(card[check][0]) <= ord('9'):
            number[int(card[check][0])-1] += 1
            number[int(card[check][0])+12] += 1
        else:
            print("Error input")
            exit()
    if dupl:
        print("Duplicate deal")
        exit()
    print(compare(kind, number))
    #print(kind)
    #print(number)
main()
#輸入5張卡片編號+數字(ex: 5H 3S 4C 7D 7H)