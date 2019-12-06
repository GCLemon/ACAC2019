# coding: utf-8

def coroutine():
    print("Hi, I am a coroutine.")
    yield
    print("If you call me once, the process stops at \"yield\" statement.")
    yield
    print("And if you call me again, then the process resumes and stops at next \"yield\" statement.")
    yield