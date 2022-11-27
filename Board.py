from subprocess import check_output
from selenium.webdriver.common.by import By
from datetime import datetime

class Board:

    @staticmethod
    def get_board(chrome):
        canv = chrome.find_element(
            By.XPATH, '/html/body/canvas'
        )
        canv.screenshot('./img/ss.png')
        return check_output('./BoardParser/cmake-build-debug/BoardParser').decode("utf-8")
