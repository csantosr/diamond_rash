import time
from selenium import webdriver
from selenium.webdriver import Keys
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.by import By

from utils import GameSetup
chrome_options = webdriver.ChromeOptions()
chrome_options.add_argument("--mute-audio")

chrome = webdriver.Chrome(chrome_options=chrome_options)
# chrome.get('https://www.miniplay.com/game/diamond-rush')
chrome.get('http://127.0.0.1:8080')
chrome.fullscreen_window()

time.sleep(1)
# GameSetup.accept_privacy(chrome)
# GameSetup.click_play(chrome)

# chrome.execute_script("document.body.style.zoom='50%'")

chrome.switch_to.frame(chrome.find_element(By.XPATH, '/html/body/iframe'))
chrome.switch_to.frame(chrome.find_element(By.XPATH, '//*[@id="game-player"]'))
canv = chrome.find_element(
        By.XPATH, '/html/body/canvas'
    )
chrome.execute_script("document.getElementsByTagName('canvas')[0].setAttribute('style',arguments[0])", 'display: block; touch-action: none; user-select: none; width: 640px; height: 960px; cursor: default; margin-left: 102px; margin-right: -101px; margin-top: 0px;')
chrome.switch_to.default_content()
i = 0

actions = ActionChains(chrome)


def press_key(key):
    chrome.switch_to.frame(chrome.find_element(By.XPATH, '/html/body/iframe'))
    chrome.switch_to.frame(chrome.find_element(By.XPATH, '//*[@id="game-player"]'))
    canv = chrome.find_element(
        By.XPATH, '/html/body/canvas'
    )
    actions.click(canv).key_down(key).pause(0.2).key_up(key).perform()
    # time.sleep(0)
    # actions.click(canv).key_up(key).perform()
    chrome.switch_to.default_content()


while True:
    s = input("====> ")
    if s == 'q':
        chrome.close()
        break
    if s == 'h':
        print('Que quiere\n')
    # Move
    if s == 'w':
        press_key(Keys.ARROW_UP)
    if s == 'a':
        press_key(Keys.ARROW_LEFT)
    if s == 's':
        press_key(Keys.ARROW_DOWN)
    if s == 'd':
        press_key(Keys.ARROW_RIGHT)

    if s == 's':
        chrome.switch_to.frame(chrome.find_element(By.XPATH, '/html/body/iframe'))
        chrome.switch_to.frame(chrome.find_element(By.XPATH, '//*[@id="game-player"]'))
        canv = chrome.find_element(
                By.XPATH, '/html/body/canvas'
            )
        canv.screenshot('img/{}.png'.format(i))
        i += 1
        chrome.switch_to.default_content()
