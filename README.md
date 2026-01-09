# StudentsUI – Programos diegimo instrukcija

Šiame repozitorijoje pateikiamas programos **išeities kodas** ir **diegimo projekto (installer) konfigūracija**.

---

## 🖥 Sistemos reikalavimai

- Windows 10 arba Windows 11
- Administratoriaus teisės (reikalingos diegimui)
- .NET Framework (jei trūks – sistema pasiūlys automatiškai)

---

## 📦 Diegimo failų gavimas

Diegimui naudojami šie failai:

- `StudentsUI.msi` **arba**
- `setup.exe`

> Pastaba: diegimo failai (`.msi`, `.exe`) **nėra saugomi GitHub repozitorijoje**.  
> Jie pateikiami atskirai kartu su projektu.

---

## ⚙️ Programos diegimas

1. Dukart spustelėkite **`StudentsUI.msi`** (arba `setup.exe`)
2. Jei sistema paprašys leidimo – pasirinkite **Yes**
3. Sekite diegimo vedlio nurodymus
4. Palaukite, kol pasirodys pranešimas **“Installation Complete”**
5. Spauskite **Close**

---

## 📁 Programos įdiegimo vieta

Po sėkmingo diegimo programa bus įdiegta į katalogą:

---

## ▶️ Programos paleidimas

1. Atidarykite aukščiau nurodytą katalogą
2. Paleiskite vykdomąjį failą:

Programa turi sėkmingai pasileisti.

---

## 🗂 Repozitorijos turinys

Šiame GitHub repozitorijoje pateikiama:

- Programos išeities kodas
- Visual Studio sprendimas (`.sln`)
- Diegimo projekto konfigūracija (`StudentsUI.vdproj`)

Nepateikiama:

- `*.exe`
- `*.msi`
- `Release/`, `Debug/` katalogai

---

## 👤 Autorius

- **Laimonas Vasiliauskas**
- Organizacija: **VVK**
