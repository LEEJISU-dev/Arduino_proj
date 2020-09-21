# 음성인식 리모컨

### 개발 기간
2017.09 ~ 2017.12 (4개월)

### 팀 구성 및 담당
3명 (상세설계 및 코드 구현)

### 주요 언어
C언어, Arduino (IR센서, 1Sheeld)

### 작업 내용
#### 1. 날씨
명령어를 말하면 LCD창에 오늘의 날씨를 출력
```c
const char weatherCommand[] = "오늘 날씨 어때";
```

![image](https://user-images.githubusercontent.com/67365433/93769973-8eb78400-fc56-11ea-849e-53018f3a696a.png)

```c
/* Call an api that will return the weather status in cairo,egypt. */
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Daejeon,KR&appid=4a31a9b440ac1403c36357cb7c4feaa2");

...

void onSuccess1(HttpResponse & response1)
{
  /* Using the response to query the Json chain till the required value. */
  /* i.e. Get the value of 'main' in the first object of the array 'weather' in the response. */
  /* Providing that the response is in JSON format. */
  response1["weather"][0]["main"].query();
  delay(200);
}

void onJsonReply1(JsonKeyChain & hell, char * output)
{
  if (hell == request1.getResponse()["weather"][0]["main"])
  {
     wt = (String)output;
    /* 1Sheeld responds using text-to-speech shield. */
    //TextToSpeech.say(output);
  }
}
```
#### 2. 시간
#### 3. 리모컨 - 스마트폰 어플 이용
#### 4. 리모컨 - 음성인식


