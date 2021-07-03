=================
Building OmegaWTK
=================

    | **Note : Before building any part of the Omega G Suite, please clone ``omega-graphics/autom``** 
    | **and follow the instructions on building autom**


1. Clone OmegaWTK 

.. code-block:: shell 

    clone https://github.com/omega-graphics/omega-wtk-project ./wtk 

2. Configure

.. code-block:: shell

    autom --mode gn --out out 

3. Build 

.. code-block:: shell 

    ninja -C out 

4. Install 

.. code-block:: shell 

    autom-install --prefix "$MY_USER_DIR/wtk"


