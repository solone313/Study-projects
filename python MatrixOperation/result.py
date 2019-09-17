import re # 문자와 숫자 추출
import sys # 시스템 강제종료
#행렬 정보
class matrix:
    name = ''
    mat = []

#파일저장
def input_matrix():
    ab = input('저장할 파일 이름을 입력해주세요 : ')
    f = open(ab+".txt", 'w')
    for p in range(3):
        a,b,c = input('행렬이름과 행렬크기를 입력해주세요 : (총 세번 입력) ').split()
        f.write(a+' '+b+' '+c)
        f.write('\n')
        print('행렬을 입력해주세요')
        for i in range(int(b)):
            data = input()
            check = data.split()
            if(len(check)!=int(c)):
                print(i,"행에서 잘못된 행렬 입력이 발생. 프로그램을 다시 실행해주세요")
                sys.exit(1)
            f.write(data)
            f.write('\n')
    print('입력완료')
    f.close()
#행렬곱 함수 
def matrix_multiplication(A, B):
    if not A :
        return B
    gop = 0
    result = 0
    an=[]
    a = len(A)
    b = len(A[0])
    c = len(B[0])
    for k in range(a):
        answer=[]
        for i in range(c):
            result = 0
            for j in range(b):
                gop = (int(A[k][j])*int(B[j][i]))
                result = result + gop
            answer.append(result)
        an.append(answer)
    return an

# 스칼라곱 함수
def scalar_multiplication(A,B):
    an = []
    a = len(A)
    b = len(A[0])
    for i in range(a):
        answer=[]
        for j in range(b):
            answer.append(int(A[i][j])*B)
        an.append(answer)
    return an

#행렬더하기 연산
def matrix_addition(A,B):
    if not A :
        return B
    an = []
    a = len(A)
    b = len(A[0])
    for i in range(a):
        answer=[]
        for j in range(b):
            answer.append(int(A[i][j])+int(B[i][j]))
        an.append(answer)
    return an

#변수선언        
result = []
main = matrix()

# 메인문 시작
cek = int(input('행렬입력 : 1 연산호출: 2\n'))
if(cek==1):
    input_matrix()
elif(cek==2):
    #파일에서 행렬읽어서 변수에 저장
    ing = input('행렬을 가져올 파일명을 입력해주세요 : ')
    f = open(ing+'.txt', 'r')
    for i in range(3):
        line = f.readline().split()
        main.name = main.name + line[0]
        abc = []
        for j in range(int(line[1])):
            line = f.readline()
            if i == 0 :
                abc.append(line.split())
            elif i== 1 :
                abc.append(line.split())
            elif i == 2 :
                abc.append(line.split())
        main.mat.append(abc)
    f.close()
    #연산 입력 후 분리
    while(True):
        ip = input("원하는 연산을 입력해주세요. (종료를 원하면 q를 입력해주세요) \n").split(' + ')
        if(ip[0]=='q'):
            sys.exit(1)
        #연산 시작
        for x in ip:
            li = re.findall("\d+", x) # 숫자 추출
            p = re.compile("[^0-9]") # 문자 추출
            ch = "".join(p.findall(x))
            com = []
            cnt = []
            for i in range(len(ch)):
                cnt.append(main.name.find(ch[i]))
            #스칼라곱 있을 경우
            if(li):
                for i in cnt:
                    com = matrix_multiplication(com,main.mat[i])
                result.append(scalar_multiplication(com,int(li[0])))
            #스칼라곱 없을 경우
            else: 
                for i in cnt:
                    com = matrix_multiplication(com,main.mat[i])
                result.append(com)
                    
                    

        #결과출력
        aaa = []
        for res in range(len(result)):
            aaa = matrix_addition(aaa,result[res])

        for z in aaa:
            for y in z:
                print(y,end=' ')
            print('\n')
else:
    print('잘못된 입력입니다 프로그램을 재실행해주세요')