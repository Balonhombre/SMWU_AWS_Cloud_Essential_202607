# ✅ [제공 코드] ipywidgets 채팅 UI — FAQChatbot과 직접 대화해보세요
import html as _html
import ipywidgets as widgets
from IPython.display import display

ui_bot = FAQChatbot()          # 위에서 완성한 챗봇 클래스 사용
_bubbles = []                  # 말풍선 HTML 누적

chat_log  = widgets.HTML()
text_in   = widgets.Text(placeholder='질문을 입력하고 Enter 또는 [전송]을 누르세요',
                         layout=widgets.Layout(width='65%'))
send_btn  = widgets.Button(description='전송',  button_style='primary')
reset_btn = widgets.Button(description='새 대화')

def _render():
    chat_log.value = (
        '<div style="height:340px; overflow-y:auto; border:1px solid #ddd; '
        'border-radius:8px; padding:12px; background:#fafafa; font-family:sans-serif;">'
        + ''.join(_bubbles) + '</div>')

def _bubble(role, text, faqs=None):
    text = _html.escape(text).replace('\n', '<br>')
    if role == 'user':   # 오른쪽 파란 말풍선
        return (f'<div style="text-align:right; margin:6px 0;"><span style="display:inline-block; '
                f'background:#3B82F6; color:#fff; padding:8px 12px; border-radius:14px 14px 2px 14px; '
                f'max-width:75%; text-align:left;">{text}</span></div>')
    src = ''
    if faqs:             # 근거 FAQ 출처 표시
        src = (f'<div style="font-size:11px; color:#888; margin-top:4px;">'
               f'📎 참조 FAQ: [{_html.escape(faqs[0]["category"])}] {_html.escape(faqs[0]["question"])}</div>')
    return (f'<div style="text-align:left; margin:6px 0;"><span style="display:inline-block; '
            f'background:#e9e9ef; color:#222; padding:8px 12px; border-radius:14px 14px 14px 2px; '
            f'max-width:75%;">🤖 {text}{src}</span></div>')

def _on_send(_):
    q = text_in.value.strip()
    if not q:
        return
    text_in.value = ''
    _bubbles.append(_bubble('user', q))
    _bubbles.append('<div style="color:#aaa; font-size:12px;">답변 생성 중...</div>')
    _render()
    answer, faqs = ui_bot.chat(q)          # ← 여기서 Bedrock 호출
    _bubbles.pop()                         # "생성 중..." 제거
    _bubbles.append(_bubble('bot', answer, faqs))
    _render()

def _on_reset(_):
    ui_bot.reset()
    _bubbles.clear()
    _bubbles.append('<div style="color:#888;">새 대화를 시작합니다. 무엇이든 물어보세요!</div>')
    _render()

send_btn.on_click(_on_send)
reset_btn.on_click(_on_reset)
import warnings
with warnings.catch_warnings():            # on_submit의 DeprecationWarning 숨김 (동작에는 문제 없음)
    warnings.simplefilter('ignore', DeprecationWarning)
    text_in.on_submit(_on_send)            # Enter 키로 전송
_on_reset(None)
display(widgets.VBox([chat_log, widgets.HBox([text_in, send_btn, reset_btn])]))
