# -*- coding: utf-8 -*-
"""
MFA 임시 자격 증명 발급 도우미
— ForceMFA 정책이 있는 계정에서 액세스 키 API 호출이 거부될 때 사용합니다.

사용법:
  1) 장기 액세스 키가 환경 변수 또는 aws configure로 설정된 상태에서 실행:
     python mfa_session.py
  2) MFA 디바이스 ARN과 인증 앱의 6자리 코드를 입력
  3) 출력된 명령을 복사해 터미널에 붙여넣은 뒤, 같은 터미널에서 앱 실행
"""
import boto3

print('IAM 콘솔 → 사용자 → 보안 자격 증명 탭 → MFA 디바이스에서 ARN을 확인하세요.')
print('예: arn:aws:iam::054422645032:mfa/teacher01\n')

mfa_arn = input('MFA 디바이스 ARN: ').strip()
code = input('인증 앱의 6자리 코드: ').strip()

creds = boto3.client('sts').get_session_token(
    SerialNumber=mfa_arn,
    TokenCode=code,
    DurationSeconds=43200,          # 12시간 유효
)['Credentials']

print('\n✅ 임시 자격 증명 발급 완료 (12시간 유효)')
print('아래 명령을 복사해 터미널에 붙여넣은 뒤, 같은 터미널에서 앱을 실행하세요.\n')

print('── Windows PowerShell ──')
print(f'$env:AWS_ACCESS_KEY_ID="{creds["AccessKeyId"]}"')
print(f'$env:AWS_SECRET_ACCESS_KEY="{creds["SecretAccessKey"]}"')
print(f'$env:AWS_SESSION_TOKEN="{creds["SessionToken"]}"')
print(f'$env:AWS_DEFAULT_REGION="us-east-1"')

print('\n── macOS / Linux ──')
print(f'export AWS_ACCESS_KEY_ID={creds["AccessKeyId"]}')
print(f'export AWS_SECRET_ACCESS_KEY={creds["SecretAccessKey"]}')
print(f'export AWS_SESSION_TOKEN={creds["SessionToken"]}')
print(f'export AWS_DEFAULT_REGION=us-east-1')
