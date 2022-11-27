from subprocess import check_output
from selenium.webdriver.common.by import By


class Solver:

    @staticmethod
    def get_steps(chrome):
        canv = chrome.find_element(
            By.XPATH, '/html/body/canvas'
        )
        canv.screenshot('./img/ss.png')
        return [*check_output('./PathFinder/cmake-build-debug/PathFinder').decode("utf-8")]
