<template>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css" />
  <div class="container">
    <header class="header">
      <div class="logo">🔔 <span>Alerts Center</span></div>
    </header>

    <!-- Filter -->
    <div class="filter-bar">
      <div class="filters">
        <button
          v-for="mode in ['All','Unread','Read']"
          :key="mode"
          :class="['filter-btn',{active:filterMode===mode}]"
          @click="filterMode=mode"
        >
          {{mode}}
        </button>
      </div>
      <button class="read-btn" @click="markAllRead">
        <i class="fa-regular fa-eye"></i> Mark All as Read
      </button>
    </div>

    <!-- Summary -->
    <section class="summary">
      <div class="summary-item danger">
        <i class="fa-solid fa-triangle-exclamation"></i>
        <span>{{ alertStats.critical ?? 0 }} Critical</span>
      </div>
      <div class="summary-item warning">
        <i class="fa-solid fa-circle-exclamation"></i>
        <span>{{ alertStats.warning ?? 0 }} Warnings</span>
      </div>
      <div class="summary-item success">
        <i class="fa-solid fa-circle-check"></i>
        <span>{{ alertStats.success ?? 0 }} Success</span>
      </div>
    </section>

    <!-- Alerts -->
    <section class="alerts-list card">
      <h3>📋 Recent Alerts</h3>
      <div v-if="filteredAlerts.length===0" class="empty">No alerts to show</div>

      <div v-for="(a,i) in filteredAlerts" :key="i" class="alert-item" :class="[a.type.toLowerCase(),{read:a.read}]">
        <div class="icon"><i :class="alertIcon(a.type)"></i></div>
        <div class="details">
          <div class="title">{{a.title}}</div>
          <div class="time">{{a.time}}</div>
          <div class="message">{{a.message}}</div>
        </div>
      </div>
    </section>
  </div>
</template>

<script>
  export default {
  data() {
    return {
      alerts: [],
      alertStats: { critical: 0, warning: 0, success: 0 },
      filterMode: "All",
    };
  },
  computed: {
    filteredAlerts() {
      if (this.filterMode === "Unread") return this.alerts.filter(a => !a.read);
      if (this.filterMode === "Read") return this.alerts.filter(a => a.read);
      return this.alerts;
    },
  },
  methods: {
    alertIcon(type) {
      switch (type) {
        case "Critical": return "fa-solid fa-triangle-exclamation";
        case "Warning": return "fa-solid fa-circle-exclamation";
        case "Success": return "fa-solid fa-circle-check";
        default: return "fa-regular fa-bell";
      }
    },
    markAllRead() {
      this.alerts.forEach(a => a.read = true);
    },
    updateStats() {
      this.alertStats = {
        critical: this.alerts.filter(a => a.type === "Critical").length,
        warning: this.alerts.filter(a => a.type === "Warning").length,
        success: this.alerts.filter(a => a.type === "Success").length,
      };
    },
  },
  mounted() {
    console.log("🟢 Alerts UI ready");

    // ✅ Watch ทั้ง object msg จาก Node-RED
  this.$watch(
  'msg',
  (newMsg) => {
  console.log("📩 Full msg received:", newMsg);
  
  // ✅ รองรับทั้ง msg.payload และ msg.payload.payload (บางกรณีซ้อน 2 ชั้น)
  const payload = newMsg?.payload?.alerts
  ? newMsg.payload
  : newMsg?.payload?.payload || {};
  
  if (payload && Array.isArray(payload.alerts)) {
  console.log("✅ Found alerts:", payload.alerts);
  this.alerts = [...payload.alerts];
  this.updateStats();
  } else {
  console.warn("⚠️ No alerts array found in payload", payload);
  }
  },
  { immediate: true, deep: true }
  );
  },
};
</script>

<style scoped>
  * {
    font-family: "Inter", sans-serif;
  }

  .container {
    max-width: 900px;
    margin: auto;
    padding: 25px 35px;
    background: white;
    border-radius: 20px;
    box-shadow: 0 3px 10px rgba(0, 0, 0, 0.05);
  }

  .logo {
    font-size: 28px;
    font-weight: bold;
    color: #27ae60;
    display: flex;
    align-items: center;
    gap: 8px;
    margin-bottom: 10px;
  }

  .filter-bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;
    flex-wrap: wrap;
    gap: 10px;
  }

  .filters {
    display: flex;
    gap: 10px;
  }

  .filter-btn {
    padding: 8px 15px;
    border-radius: 8px;
    border: 1px solid #ccc;
    background: #fff;
    font-weight: 600;
    cursor: pointer;
    transition: 0.2s;
  }

  .filter-btn.active {
    background: #27ae60;
    color: white;
    border-color: #27ae60;
  }

  .filter-btn:hover {
    background: #eafaf1;
  }

  .read-btn {
    border: none;
    background: #3498db;
    color: white;
    padding: 10px 15px;
    border-radius: 8px;
    font-weight: 600;
    cursor: pointer;
    transition: 0.2s;
  }

  .read-btn:hover {
    background: #2980b9;
  }

  .summary {
    display: flex;
    justify-content: space-around;
    margin-bottom: 20px;
    flex-wrap: wrap;
    gap: 10px;
  }

  .summary-item {
    padding: 10px 20px;
    border-radius: 12px;
    color: white;
    font-weight: 600;
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 22px;
  }

  .danger {
    background: #e74c3c;
  }

  .warning {
    background: #f39c12;
  }

  .success {
    background: #2ecc71;
  }

  .alerts-list {
    max-height: 300px;
    overflow-y: auto;
    overflow-x: hidden;
    scrollbar-width: thin;
  }

  .alert-item {
    display: flex;
    align-items: flex-start;
    gap: 15px;
    padding: 12px;
    border-radius: 10px;
    margin-bottom: 10px;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.05);
    transition: 0.3s;
  }

  .alert-item.critical {
    border-left: 6px solid #e74c3c;
    background: #fdecea;
  }

  .alert-item.warning {
    border-left: 6px solid #f39c12;
    background: #fff5e6;
  }

  .alert-item.success {
    border-left: 6px solid #2ecc71;
    background: #ecf9f1;
  }

  .alert-item.read {
    opacity: 0.6;
    filter: grayscale(40%);
  }

  .details .title {
    font-weight: 600;
    font-size: 16px;
  }

  .details .time {
    color: #888;
    font-size: 13px;
  }

  .details .message {
    margin-top: 5px;
  }

  .empty {
    text-align: center;
    padding: 20px;
    color: #888;
  }

  /* ✅ Responsive (iPhone SE) */
  @media (max-width: 400px) {
    .container {
      padding: 15px;
      border-radius: 12px;
    }

    .logo {
      font-size: 22px;
      margin-bottom: 10px;
    }

    .filter-bar {
      flex-direction: column;
      align-items: stretch;
      gap: 8px;
    }

    .filters {
      justify-content: space-between;
    }

    .filter-btn {
      flex: 1;
      font-size: 14px;
      padding: 6px 10px;
    }

    .read-btn {
      width: 100%;
      font-size: 14px;
      padding: 8px;
    }

    .summary {
      flex-direction: column;
      align-items: stretch;
    }

    .summary-item {
      justify-content: center;
      font-size: 18px;
    }

    .alerts-list {
      max-height: 240px;
    }

    .alert-item {
      flex-direction: column;
      gap: 8px;
    }
  }
</style>