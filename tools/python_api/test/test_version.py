def test_version() -> None:
    import gorgonzola

    assert gorgonzola.version != ""
    assert gorgonzola.storage_version > 0
    assert gorgonzola.version == gorgonzola.__version__
